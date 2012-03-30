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

#define BUFFERSIZE 256

enum {

  STATE_CLOSED,
  STATE_OPEN,
  STATE_INITIAL_READ,
  STATE_READING,
  STATE_EOS
};

hbcn_curl::curl_input_stream::curl_input_stream(const char* url) {

  pthread_mutex_init(&waiting_mutex, 0);
  pthread_cond_init(&read_cond, 0);
  pthread_cond_init(&write_cond, 0);
  
  this->url = url;
  
  buffer = new std::deque<unsigned char>();
  
  stream_state = STATE_CLOSED;
}

hbcn_curl::curl_input_stream::~curl_input_stream() {

  pthread_mutex_destroy(&waiting_mutex);
  pthread_cond_destroy(&read_cond);
  pthread_cond_destroy(&write_cond);
  
  if (buffer)
  {
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

  for (int i = 0; i < nitems; i++)
  {
    if (buffer->size() == BUFFERSIZE)
    {
      pthread_mutex_lock(&waiting_mutex);
      pthread_cond_signal(&read_cond);
      pthread_cond_wait(&write_cond, &waiting_mutex);
      pthread_mutex_unlock(&waiting_mutex);
    }
    
    buffer->push_back(data[i]);
  }

  pthread_mutex_lock(&waiting_mutex);
  pthread_cond_signal(&read_cond);
  pthread_mutex_unlock(&waiting_mutex);
  return size * nitems;
}

void* hbcn_curl::thread_that(void* cookie) {

  curl_input_stream* input_stream = (curl_input_stream*) cookie;
  input_stream->thread_this();
}

void curl_input_stream::thread_this() {

  CURL* curl = curl_easy_init();
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, BUFFERSIZE);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &hbcn_curl::buffer_input);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
  
  stream_state = STATE_INITIAL_READ;
  
  try
  {
    curl_easy_perform(curl);
  }
  catch(...)
  {
    // do nothing
  }
  
  stream_state = STATE_EOS;
  
  curl_easy_cleanup(curl);

  pthread_mutex_lock(&waiting_mutex);
  pthread_cond_signal(&read_cond);
  pthread_mutex_unlock(&waiting_mutex);
}

/**
 * @return The number of bytes read or -1 if the end of the stream has been reached.
 */
int hbcn_curl::curl_input_stream::read(unsigned char* data, int length) {

  if (length <= 0) return 0;
  else if (stream_state == STATE_EOS && buffer->empty()) return -1;
  else if (stream_state == STATE_CLOSED)
  {
    stream_state = STATE_OPEN;

    pthread_t pthread;
    pthread_create(&pthread, 0, &hbcn_curl::thread_that, this);

    pthread_mutex_lock(&waiting_mutex);
    pthread_cond_signal(&write_cond);
    pthread_cond_wait(&read_cond, &waiting_mutex);
    pthread_mutex_unlock(&waiting_mutex);
  }

  int i;

  for (i = 0; i < length; i++)
  {
    if (stream_state != STATE_EOS && buffer->empty())
    {
        pthread_mutex_lock(&waiting_mutex);
        pthread_cond_signal(&write_cond);
        pthread_cond_wait(&read_cond, &waiting_mutex);
        pthread_mutex_unlock(&waiting_mutex);
    }

    if (buffer->empty()) break;

    data[i] = buffer->front();
    buffer->pop_front();
  }

  pthread_mutex_lock(&waiting_mutex);
  pthread_cond_signal(&write_cond);
  pthread_mutex_unlock(&waiting_mutex);

  return i;
}
