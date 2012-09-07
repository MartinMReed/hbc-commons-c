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

#ifndef HBC_CIRCULAR_BUFFER_H
#define HBC_CIRCULAR_BUFFER_H

#include <sys/types.h>

namespace hbc
{

class circular_buffer
{
public:

    circular_buffer(ssize_t size);
    virtual ~circular_buffer();

    ssize_t read(unsigned char* buf, ssize_t size);
    ssize_t write(unsigned char* buf, ssize_t size);

    ssize_t size();
    ssize_t capacity();
    bool empty();

private:

    unsigned char* m_buffer;
    ssize_t m_capacity, m_size, m_start;
};

}

#endif
