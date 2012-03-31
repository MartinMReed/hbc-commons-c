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

#ifndef HBC_FILE_INPUT_STREAM_H
#define HBC_FILE_INPUT_STREAM_H

#include "input_stream.h"
using namespace hbc;

#include <stdio.h>
#include <pthread.h>

namespace hbc {

  class file_input_stream : public input_stream {
  
    public:
    
    file_input_stream(const char* path);
    ~file_input_stream();
    
    int read(unsigned char* data, int length);
    
    private:
    
    const char* path;
    FILE* file;
    
    pthread_mutex_t mutex;
    
    int stream_state;
  };
}

#endif
