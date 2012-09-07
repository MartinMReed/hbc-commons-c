/**
 * Copyright (c) 2012 Martin M Reed
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

#include "circular_buffer.h"

#include <stdio.h>
#include <algorithm>

hbc::circular_buffer::circular_buffer(ssize_t capacity)
{
    m_buffer = (unsigned char*) malloc(capacity * sizeof(unsigned char));
    m_capacity = capacity;
    m_start = 0;
    m_size = 0;
}

hbc::circular_buffer::~circular_buffer()
{
    free(m_buffer);
}

ssize_t hbc::circular_buffer::capacity()
{
    return m_capacity;
}

ssize_t hbc::circular_buffer::size()
{
    return m_size;
}

bool hbc::circular_buffer::empty()
{
    return m_size == 0;
}

ssize_t hbc::circular_buffer::read(unsigned char* buf, ssize_t len)
{
    if (m_size <= 0 || len <= 0) return 0;
    len = std::min(len, m_size);

    ssize_t read = 0;
    do
    {
        ssize_t r = std::min(m_capacity - m_start, len - read);
        memcpy(&buf[read], &m_buffer[m_start], r);
        memset(&m_buffer[m_start], 'x', r);
        m_start = (m_start + r) % m_capacity;
        read += r;
    }
    while (read < len);

    m_size -= read;

    return read;
}

ssize_t hbc::circular_buffer::write(unsigned char* buf, ssize_t len)
{
    ssize_t end = (m_start + m_size) % m_capacity;

    ssize_t write = 0;
    do
    {
        ssize_t w = std::min(m_capacity - end, len - write);
        memcpy(&m_buffer[end], &buf[write], w);
        end = (end + w) % m_capacity;
        write += w;
    }
    while (write < len);

    if (m_size + len > m_capacity) m_start = end % m_capacity;

    m_size = std::min(m_size + (len % m_capacity), m_capacity);

    return write;
}
