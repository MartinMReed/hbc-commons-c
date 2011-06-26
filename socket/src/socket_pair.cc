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

#include "socket_server.h"

#include <strings.h>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>

/**
 *
 */
hbcn_socket::socket_pair::socket_pair(socket_server* socket, int fd) {

  this->socket = socket;
  this->fd = fd;
}

/**
 *
 */
hbcn_socket::socket_pair::~socket_pair() {

  disconnect();
}

/**
 *
 */
void hbcn_socket::socket_pair::disconnect() {

  if (fd) {
  
    close(fd);
    fd = 0;
  }
}

/**
 *
 */
void hbcn_socket::socket_pair::start() {
  
  while (read());
}
