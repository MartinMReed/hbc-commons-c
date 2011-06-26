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

#ifndef ARD_SOCKET_H
#define ARD_SOCKET_H

#include "libhbc_socket/socket_burst.h"

namespace hbcn_arduino {

  class ard_socket {
  
    public:
    
    virtual ~ard_socket();
    
    int (*acknak)(const unsigned char* payload, int length, void* cookie);
    void* acknak_cookie;
    
    virtual void begin(long baud) = 0;
    virtual int available();
    virtual unsigned char serial_read() = 0;
    virtual int serial_write(unsigned char* buffer, int len) = 0;
    
    int read();
    void write(const unsigned char* payload, int length);
    
    protected:
    
    ard_socket();
    
    private:
    
    unsigned char* read_buffer;
    unsigned char* write_buffer;
    
    int write(const unsigned char* payload, int length, int offset);
    
    unsigned char ensured_read();
    int read(unsigned char* buffer);
  };
}

#endif
