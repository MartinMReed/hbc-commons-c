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

#ifndef HBC_SOCKET_SERVER_H
#define HBC_SOCKET_SERVER_H

#include "socket.h"

namespace hbcn_socket {

  class socket_pair;
  
  void* start_pairing(void* cookie);

  class socket_server {
  
    friend class socket_pair;
    friend void* start_pairing(void* cookie);
    
    public:
    
    socket_server(int port);
    ~socket_server();
    
    void connect();
    void start();
    
    socket_pair* (*create_pair)(socket_server* socket, int fd);
    
    private:
    
    int accept_socket();
    
    int port;
    int fd;
  };

  class socket_pair : public hbc_socket{
  
    friend void* start_pairing(void* cookie);
    
    public:
    
    socket_pair(socket_server* socket, int fd);
    ~socket_pair();
    
    void start();
    void disconnect();
    
    private:
    
    socket_server* socket;
  };
}

#endif
