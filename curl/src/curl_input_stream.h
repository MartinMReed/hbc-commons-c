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

#ifndef HBC_CURL_INPUT_STREAM_H
#define HBC_CURL_INPUT_STREAM_H

#include <curl/curl.h>
#include <pthread.h>

#include "libhbc_stdio/input_stream.h"
using namespace hbcn_stdio;

#include "libhbc_collections/bounded_buffer.h"
using namespace hbcn_collections;

namespace hbcn_curl {

  class curl_input_stream;
  
  void* thread_that(void* cookie);
  size_t buffer_input(unsigned char* data, size_t size, size_t nitems, curl_input_stream* input_stream);
  
  class curl_input_stream : public input_stream {
  
    friend void* thread_that(void* cookie);
    friend size_t buffer_input(unsigned char* data, size_t size, size_t nitems, curl_input_stream* input_stream);
    
    public:
    
    curl_input_stream(const char* url);
    ~curl_input_stream();
    
    int read(unsigned char* data, int length);
    
    private:
    
    size_t buffer_input(unsigned char* data, size_t size, size_t nitems);
    
    void thread_this();
    
    pthread_rwlock_t rwlock;
    pthread_mutex_t waiting_mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
    
    const char* url;
    
    hbcn_collections::buffer<unsigned char>* buffer;
    
    int stream_state;
  };
}

#endif
