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

#include "socket_client.h"

#include <netdb.h>

#include <strings.h>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>

/**
 * Set up the server connection by opening the socket and binding to
 * the given port address.
 *
 * @param port The port address to bind to.
 */
hbcn_socket::socket_client::socket_client(const char* hostname, int port) {

  this->hostname = hostname;
  this->port = port;
}

/**
 * Destroy the server and close the socket.
 */
hbcn_socket::socket_client::~socket_client() {

  hostname = 0;
  port = 0;
}

/**
 *
 */
void hbcn_socket::socket_client::connect() {

  // open a socket to listen on
  fd = ::socket(AF_INET, SOCK_STREAM, 0);
  
  if (fd < 0) {
  
    throw "opening socket";
  }
  
  struct hostent* host = gethostbyname(hostname);
  
  if (!host) {
  
    throw "no such host";
  }
  
  struct sockaddr_in s_addr;
  bzero((char*) &s_addr, sizeof(s_addr));
  
  s_addr.sin_family = AF_INET;
  bcopy((char*) host->h_addr, (char*) &s_addr.sin_addr.s_addr, host->h_length);
  s_addr.sin_port = htons(port);
  
  if (::connect(fd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
  
    throw "connecting";
  }
}

/**
 *
 */
void hbcn_socket::socket_client::start() {
  
  while (read());
}