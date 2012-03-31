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

#ifndef HBC_STRING_BUFFER_H
#define HBC_STRING_BUFFER_H

#include "vector_buffer.h"

namespace hbc {

  class string_buffer {
  
    public:
    
    string_buffer();
    ~string_buffer();
    
    char* to_string();
    char* to_string(int start, int end);
    
    void append(const unsigned char& c);
    void append(const char& c);
    void append(const unsigned char* c, int length);
    void append(const char* c, int length);
    
    void prepend(const unsigned char& c);
    void prepend(const char& c);
    void prepend(const unsigned char* c, int length);
    void prepend(const char* c, int length);
    
    void trim();
    void pop_back();
    void pop_front();
    
    const unsigned char operator [](const int index);
    const unsigned char first() { return (*this)[0]; }
    const unsigned char last() { return (*this)[size()-1]; }
    
    int size();
    int empty() { return size() == 0; }
    
    private:
    
    vector_buffer<unsigned char> m_buffer;
  };
}

#endif
