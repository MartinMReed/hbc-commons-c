/**
 * Copyright (c) 2009-2012 Martin M Reed
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

#ifndef HBC_CHUNK_H
#define HBC_CHUNK_H

#include "../curl/_curl.h"

namespace hbc {

  size_t chunk_write(void* buffer, size_t size, size_t nitems, void* stream);
  
  class chunker : public curler {
  
    friend size_t chunk_write(void* buffer, size_t size, size_t nitems, void* stream);
    
    public:
    
    chunker(const char* url, const char* directory, int chunk_size);
    ~chunker();
    
    void (*on_create_new_chunk)(void* stream);
    void (*on_close_chunk)(void* stream);
    
    int get_file_id() { return file_id; }
    int get_size() { return size; }
    
    protected:
    
    void init();
    void cleanup();
    
    private:
    
    void create_new_chunk();
    void close_chunk();
    
    int file_id;
    int size;
    FILE* file;
    
    const char* directory;
    int chunk_size;
  };
}

#endif
