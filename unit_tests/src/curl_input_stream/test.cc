/**
 * Copyright (c) 2010-2012 Martin M Reed
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

#include <string.h>

int curl_input_stream_test() {
  
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com/intl/en_ALL/images/srpr/logo1w.png");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

  hbc::curl_input_stream input_stream(curl);
  
  unsigned char buffer[7330];
  
  int total = 0;
  
  int read;
  while ((read = input_stream.read(buffer+total, 256)) != -1) {
  
    total += read;
  }

  curl_easy_cleanup(curl);
  
  FILE* file = fopen("test.png", "w");
  fwrite(buffer, sizeof(unsigned char), total, file);
  fclose(file);
  
  printf("actual[%i], expected[7330]\n", total);
  
  md5_state_t state;
  md5_byte_t digest[16];
  
  md5_init(&state);
  md5_append(&state, (const md5_byte_t*)buffer, total);
  md5_finish(&state, digest);
  
  char hex_output[33];
  hex_output[32] = 0;
  
  for (int i = 0; i < 16; i++) {
  
    sprintf(hex_output+(i*2), "%02x", digest[i]);
  }
  
  printf("actual[%s], expected[e04c18d9aa6931e0a7e8cf91245aa791]\n", hex_output);
  
  if (strcmp(hex_output, "e04c18d9aa6931e0a7e8cf91245aa791") != 0) {
  
    throw -1;
  }
  
  return 0;
}
