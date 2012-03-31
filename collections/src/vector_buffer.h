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

#ifndef HBC_VECTOR_BUFFER_H
#define HBC_VECTOR_BUFFER_H

#include <vector>

#include "buffer.h"

namespace hbc {

  template<class T> class vector_buffer : public buffer<T> {
  
    public:
    
    vector_buffer();
    ~vector_buffer();
    
    const T& operator[](const int index);
    
    void push_back(const T& object);
    void push_front(const T& object);
    
    void pop_back();
    void pop_front();
    
    void remove(int index);
    
    int capacity() { return buffer.max_size(); }
    int size() { return buffer.size(); }
    
    private:
    
    vector<T> buffer;
  };
}

#include "vector_buffer_impl.h"

#endif
