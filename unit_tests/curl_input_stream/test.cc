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

#include "../libhbc_curl/curl_input_stream.h"
using namespace hbcn_curl;

#include <string>

int curl_input_stream_test() {
  
  const char* url = "http://www.google.com/intl/en_ALL/images/srpr/logo1w.png";
  input_stream* input_stream = new curl_input_stream(url);
  
  unsigned char buffer[7409];
  
  int total = 0;
  
  int read;
  while ((read = input_stream->read(buffer+total, 256)) != -1) {
  
    total += read;
  }
  
  FILE* file = fopen("test.png", "w");
  fwrite(buffer, sizeof(unsigned char), total, file);
  fclose(file);
  
  printf("actual[%i], expected[7409]\n", total);
  
  md5_state_t state;
  md5_byte_t digest[16];
  
  md5_init(&state);
  md5_append(&state, (const md5_byte_t*)buffer, 7409);
  md5_finish(&state, digest);
  
  char hex_output[33];
  hex_output[32] = 0;
  
  for (int i = 0; i < 16; i++) {
  
    sprintf(hex_output+(i*2), "%02x", digest[i]);
  }
  
  printf("actual[%s], expected[47a50c26d5a8a46a13ec6f24290b5d0c]\n", hex_output);
  
  if (strcmp(hex_output, "47a50c26d5a8a46a13ec6f24290b5d0c") != 0) {
  
    throw -1;
  }
  
  return 0;
}