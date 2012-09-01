/**
 * Copyright (c) 2009-2012 Martin M Reed
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

// the size of the payload is store in a flag, which
// means that the payload size is limited to the max
// value that can fit within that flag. the flag is one byte.
#define PAYLOAD_MAX 255

// the max burst size will be the max size of the payload
// plus the number of flags that come before the payload.
// use the payload index to determine how many flags are set.
#define BURST_MAX PAYLOAD_MAX + socket_burst_EOH

namespace hbc
{

int is_open(int fd);
int open(const char* hostname, int port);
void write(int fd, const unsigned char* payload, int length);
int write(int fd, const unsigned char* payload, int length, int offset);

// structure of a packet-burst header and payload
enum
{
    socket_burst_size, // header, one byte
    socket_burst_chunked, // header, one byte
    socket_burst_payload, // remaining bytes
    socket_burst_EOH = socket_burst_payload // End-of-Header marker
};

class socket;

typedef struct
{
    int length;
    unsigned char* payload;
    socket* sock;
} socket_packet;

void free_packet(socket_packet* packet);

class socket
{

public:

    virtual ~socket();

    int (*acknak)(socket_packet* packet, void* cookie);
    void* acknak_cookie;

    int read();
    void write(const unsigned char* payload, int length);

    void disconnect();

    void adopt()
    {
        adopted = true;
    }

    int fd;

protected:

    socket();

    bool adopted;

private:

    int read_packet();

    void assert_fd_set();

    unsigned char* read_buffer;
};

}

#endif
