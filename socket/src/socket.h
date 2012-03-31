/**
 * Copyright (c) 2009-2010 Martin M Reed
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

#ifndef HBC_SOCKET_H
#define HBC_SOCKET_H

#include "socket_burst.h"

namespace hbc {

  class hbc_socket {
  
    public:
    
    virtual ~hbc_socket();
    
    int (*acknak)(const unsigned char* payload, int length, void* cookie);
    void* acknak_cookie;
    
    int read();
    void write(const unsigned char* payload, int length);
    
    void disconnect();
    
    protected:
    
    hbc_socket();
    
    int fd;
    
    private:
    
    unsigned char* read_buffer;
    unsigned char* write_buffer;
    
    int write(const unsigned char* payload, int length, int offset);
    
    void assert_fd_set();
  };
}

#endif
