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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "libhbc_math/math.h"
using namespace hbc;

#include "socket.h"

/**
 *
 */
hbc::hbc_socket::hbc_socket() {

  read_buffer = new unsigned char[BURST_MAX];
  write_buffer = new unsigned char[BURST_MAX];
}

/**
 *
 */
hbc::hbc_socket::~hbc_socket() {

  disconnect();
  
  delete read_buffer;
  read_buffer = 0;
  
  delete write_buffer;
  write_buffer = 0;
  
  acknak = 0;
  acknak_cookie = 0;
}

/**
 *
 */
void hbc::hbc_socket::disconnect() {

  if (fd) {
  
    close(fd);
    fd = 0;
  }
}

/**
 *
 */
void hbc::hbc_socket::assert_fd_set() {

  if (!fd) {
  
    throw "file descriptor is not set";
  }
}

/**
 * Write a payload out to the given socket.
 *
 * @param fd The socket to write to.
 * @param payload The packet-burst payload.
 * @param length The byte count of the payload parameter.
 */
void hbc::hbc_socket::write(const unsigned char* payload, int length) {

  assert_fd_set();
  
  if (length > PAYLOAD_MAX) {
  
    int buffer_length = num_digits(length);
    unsigned char buffer[buffer_length];
    sprintf((char*)buffer, "%i", length);
    write(buffer, length, buffer_length);
  }
  
  for (int i = 0; i < length; i += write(payload, length, i));
  
  printf("<- fd[%i], chunked[%i], %s\n", fd, length > PAYLOAD_MAX, payload);
}

/**
 * Write a payload out to the given socket.
 *
 * @param fd The socket to write to.
 * @param payload The packet-burst payload.
 * @param length The byte count of the payload parameter.
 */
int hbc::hbc_socket::write(const unsigned char* payload, int length, int offset) {

  assert_fd_set();
  
  // if we add PAYLOAD_MAX to the length of what we have already sent
  // and it is greater than the total length, that means we are at
  // the last packet and can send less than PAYLOAD_MAX.
  int payload_size = (offset + PAYLOAD_MAX > length) ? length - offset : PAYLOAD_MAX;
  write_buffer[socket_burst_size] = payload_size;
  
  // if we add the current burst length to what we have already sent,
  // and it is less than the total length, that means we still have
  // more packets to send, so set the chunked flag to tell the recipient
  // to keep reading our other packets.
  write_buffer[socket_burst_chunked] = offset + payload_size < length;
  
  memcpy(write_buffer + socket_burst_EOH, payload, payload_size);
  
  if (::write(fd, write_buffer, sizeof(unsigned char) * (payload_size + socket_burst_EOH)) < 0) {
  
    throw "writing to socket";
  }
  
  return payload_size;
}

/**
 * Read from the given socket.
 *
 * @return true if the Client/Server should continue
 * their conversation.
 */
int hbc::hbc_socket::read() {

  assert_fd_set();
  
  bzero(read_buffer, BURST_MAX);

  int read_len = ::read(fd, (char*)read_buffer, BURST_MAX);
  
  if (read_len < 0) {

    throw "reading from socket";
  }
  else if (read_len == 0) {
  
    return 0;
  }
  else if (read_len != read_buffer[socket_burst_size] + socket_burst_EOH) {
  
    throw "did not read full socket burst";
  }
  
  int payload_length = read_buffer[socket_burst_size];
  unsigned char* payload = new unsigned char[payload_length + 1];
  memcpy(payload, read_buffer + socket_burst_EOH, payload_length);
  payload[payload_length] = 0;
  
  if (read_buffer[socket_burst_chunked]) {
    
    // if the bursts are chunked, the first burst will be
    // the total size of all the chunks combined
    payload_length = atoi((char*)payload);
    
    // delete the first burst payload since we
    // do not need it once we have the total length
    delete payload;
    
    payload = new unsigned char[payload_length];
    bzero(payload, payload_length);
    
    int payload_offset = 0;
    
    // continue reading until the last chunk as been sent.
    // the last chunk will specify a 0 flag in the chunk flag
    // which will disable this loop.
    while (read_buffer[socket_burst_chunked] && ::read(fd, (unsigned char*)read_buffer, BURST_MAX)) {
    
      memcpy(payload + payload_offset, read_buffer + socket_burst_EOH, read_buffer[socket_burst_size]);
      payload_offset += read_buffer[socket_burst_size];
    }
    
    if (payload_offset != payload_length) {
    
      throw "did not read full chunked socket burst";
    }
  }

  printf("-> fd[%i], %s\n", fd, payload);

  if (acknak) {
  
    return acknak(payload, payload_length, acknak_cookie);
  }
  
  return 1;
}
