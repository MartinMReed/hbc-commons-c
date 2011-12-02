/**
 * Copyright (c) 2010 Martin M Reed
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

#include "test.h"

#include "../libhbc_crypto/md5.h"

#include "../libhbc_stdio/file_input_stream.h"
using namespace hbcn_stdio;

#include <string.h>

int file_input_stream_test() {
  
  file_input_stream input_stream("tinyxml/utf8test.xml");
  
  int total = 0;
  unsigned char buffer[690];
  
  int read;
  while ((read = input_stream.read(buffer+total, 256)) != -1) {
  
    total += read;
  }
  
  printf("actual[%i], expected[690]\n", total);
  
  md5_state_t state;
  md5_byte_t digest[16];
  
  md5_init(&state);
  md5_append(&state, (const md5_byte_t*)buffer, 690);
  md5_finish(&state, digest);
  
  char hex_output[33];
  hex_output[32] = 0;
  
  for (int i = 0; i < 16; i++) {
  
    sprintf(hex_output+(i*2), "%02x", digest[i]);
  }
  
  printf("actual[%s], expected[60125088e7992e07e224685e1c9fca6f]\n", hex_output);
  
  if (strcmp(hex_output, "60125088e7992e07e224685e1c9fca6f") != 0) {
  
    throw -1;
  }
  
  return 0;
}
