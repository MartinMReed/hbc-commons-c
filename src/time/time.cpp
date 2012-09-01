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

#include <time.h>

#include "time.h"

namespace hbc {

  int __nsleep(const struct timespec* req) {
  
    struct timespec rem;
    
    if(nanosleep(req, &rem) == -1) {
      __nsleep(&rem);
    }
    else {
      return 1;
    }
  }
}

int hbc::milisleep(unsigned long miliseconds) {

  long remainder = miliseconds % 1000L;
  
  struct timespec req;
  req.tv_sec = (miliseconds - remainder) / 1000L;
  req.tv_nsec = remainder * 1000000L;
  __nsleep(&req);
  
  return 1;
}
