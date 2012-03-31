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

#include "file_input_stream.h"

#define STATE_CLOSED 0
#define STATE_OPEN 1
#define STATE_EOS 4

/*
 *
 */
hbc::file_input_stream::file_input_stream(const char* path) {

  this->path = path;
  
  pthread_mutex_init(&mutex, 0);
  
  stream_state = STATE_CLOSED;
}

/*
 *
 */
hbc::file_input_stream::~file_input_stream() {

  if (file) {
  
    fclose(file);
    file = 0;
  }
  
  pthread_mutex_destroy(&mutex);
}

/**
 * 
 * @return The number of bytes read or -1 if the end of the stream has been reached.
 */
int hbc::file_input_stream::read(unsigned char* data, int length) {

  if (length <= 0) {
  
    return 0;
  }
  
  pthread_mutex_lock(&mutex);
  
  if (stream_state == STATE_EOS) {
  
    pthread_mutex_unlock(&mutex);
    
    // all of the bytes have been read and
    // the end of the stream has been reached
    return -1;
  }
  
  if (stream_state == STATE_CLOSED) {
  
    if (!(file = fopen(path, "r"))) {
    
      stream_state = STATE_EOS;
      pthread_mutex_unlock(&mutex);
      return -1;
    }
    
    stream_state = STATE_OPEN;
  }
  
  int result = fread(data, sizeof(unsigned char), length, file);
  
  if (!result) {
  
    stream_state = STATE_EOS;
    
    fclose(file);
    file = 0;
  }
  
  pthread_mutex_unlock(&mutex);
  return result;
}
