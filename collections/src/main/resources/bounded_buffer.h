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

#ifndef HBC_BOUNDED_BUFFER_H
#define HBC_BOUNDED_BUFFER_H

#include "buffer.h"

namespace hbcn_collections {

  template<class T> class bounded_buffer : public buffer<T> {
  
    public:
    
    bounded_buffer(int capacity);
    ~bounded_buffer();
    
    const T& operator[](const int index);
    
    void push_back(const T& object);
    void push_front(const T& object);
    
    void pop_back();
    void pop_front();
    
    void remove(int index);
    
    int capacity() { return m_capacity; }
    int size() { return m_size; }
    
    private:
    
    inline int translate(int index);
    inline void increment_start_index();
    inline void decrement_start_index();
    
    T* m_buffer;
    int m_size;
    int m_capacity;
    int start_index;
  };
}

#include "bounded_buffer_impl.h"

#endif
