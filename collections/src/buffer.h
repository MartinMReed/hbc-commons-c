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

#ifndef HBC_BUFFER_H
#define HBC_BUFFER_H

#include <stdlib.h>
using namespace std;

namespace hbcn_collections {

  template<class T> class buffer {
  
    public:
    
    virtual void push_back(const T& object) = 0;
    virtual void push_front(const T& object) = 0;
    
    virtual void pop_back() = 0;
    virtual void pop_front() = 0;
    
    virtual void remove(int index) = 0;
    
    virtual const T& operator [](const int index) = 0;
    const T& first() { return (*this)[0]; }
    const T& last() { return (*this)[size()-1]; }
    
    virtual int capacity() = 0;
    virtual int size() = 0;
    int empty() { return size() == 0; }
    int full() { return size() == capacity(); }
  };
}

#endif
