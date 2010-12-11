/**
 * Copyright (c) 2010 Martin M Reed
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "curl_input_stream.h"
using namespace hbcn_curl;

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

enum {

  STATE_CLOSED,
  STATE_OPEN,
  STATE_INITIAL_READ,
  STATE_READING,
  STATE_EOS
};

/*
 *
 */
hbcn_curl::curl_input_stream::curl_input_stream(const char* url) {

  pthread_rwlock_init(&rwlock, 0);
  pthread_mutex_init(&waiting_mutex, 0);
  pthread_cond_init(&read_cond, 0);
  pthread_cond_init(&write_cond, 0);
  
  this->url = url;
  
  buffer = new bounded_buffer<unsigned char>(256);
  
  stream_state = STATE_CLOSED;
}

/*
 *
 */
hbcn_curl::curl_input_stream::~curl_input_stream() {

  pthread_rwlock_destroy(&rwlock);
  pthread_mutex_destroy(&waiting_mutex);
  pthread_cond_destroy(&read_cond);
  pthread_cond_destroy(&write_cond);
  
  if (buffer) {
  
    delete buffer;
    buffer = 0;
  }
}

size_t hbcn_curl::buffer_input(unsigned char* data, size_t size, size_t nitems, curl_input_stream* input_stream) {

  return input_stream->buffer_input(data, size, nitems);
}

size_t curl_input_stream::buffer_input(unsigned char* data, size_t size, size_t nitems) {

  if (stream_state == STATE_INITIAL_READ) {
  
    stream_state = STATE_READING;
  }
  else {
  
    pthread_cond_wait(&write_cond, &waiting_mutex);
  }
  
  pthread_rwlock_wrlock(&rwlock);
  
  for (int i = 0; i < nitems; i++) {
  
    if (buffer->full()) {
    
      pthread_cond_signal(&read_cond);
      pthread_rwlock_unlock(&rwlock);
      pthread_cond_wait(&write_cond, &waiting_mutex);
      pthread_rwlock_wrlock(&rwlock);
    }
    
    buffer->push_back(data[i]);
  }
  
  pthread_cond_signal(&read_cond);
  pthread_rwlock_unlock(&rwlock);
  return size * nitems;
}

/**
 * 
 */
void* hbcn_curl::thread_that(void* cookie) {

  curl_input_stream* input_stream = (curl_input_stream*) cookie;
  input_stream->thread_this();
}

/**
 * 
 */
void curl_input_stream::thread_this() {

  CURL* curl = curl_easy_init();
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, buffer->capacity());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &hbcn_curl::buffer_input);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
  
  stream_state = STATE_INITIAL_READ;
  
  try {
  
    curl_easy_perform(curl);
  }
  catch(...) {
  
    // do nothing
  }
  
  stream_state = STATE_EOS;
  
  curl_easy_cleanup(curl);
  
  pthread_cond_signal(&read_cond);
  pthread_rwlock_unlock(&rwlock);
}

/**
 * 
 * @return The number of bytes read or -1 if the end of the stream has been reached.
 */
int hbcn_curl::curl_input_stream::read(unsigned char* data, int length) {

  if (length <= 0) {
  
    return 0;
  }
  
  pthread_rwlock_rdlock(&rwlock);
  
  if (stream_state != STATE_EOS) {
  
    int state_closed = stream_state == STATE_CLOSED;
    
    if (state_closed || buffer->empty()) {
    
      if (state_closed) {
      
        stream_state = STATE_OPEN;
        
        pthread_t pthread;
        pthread_create(&pthread, 0, &hbcn_curl::thread_that, this);
      }
      else {
      
        pthread_cond_signal(&write_cond);
      }
      
      pthread_rwlock_unlock(&rwlock);
      pthread_cond_wait(&read_cond, &waiting_mutex);
      pthread_rwlock_rdlock(&rwlock);
    }
  }
  
  if (buffer->empty() && stream_state == STATE_EOS) {
  
    pthread_rwlock_unlock(&rwlock);
    
    // all of the bytes have been read and
    // the end of the stream has been reached
    return -1;
  }
  
  int i;
  
  for (i = 0; !buffer->empty() && i < length; i++) {
  
    data[i] = (*buffer)[0];
    buffer->pop_front();
  }
  
  pthread_rwlock_unlock(&rwlock);
  
  return i;
}