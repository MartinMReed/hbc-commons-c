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

#include <curl/curl.h>
#include <math.h>
#include <sys/stat.h>
#include <string.h>

#include "libhbc_math/math.h"
using namespace hbc;

#include "chunk.h"

size_t hbc::chunk_write(void* buffer, size_t size, size_t nitems, void* stream) {

  chunker* chunker = (struct chunker*) stream;
  
  FILE* file = chunker->file;
  
  int chunk_size = chunker->chunk_size;
  int target_size = chunker->size + nitems;
  
  int overflow = (chunk_size >= target_size) ? 0 : target_size - chunk_size;
  int available = nitems - overflow;
  
  fwrite(buffer, size, available, file);
  chunker->size += available;
  
  if (overflow > 0) {
  
    chunker->create_new_chunk();
    
    fwrite(buffer, size, overflow, file);
    chunker->size += overflow;
  }

  return size * nitems;
}

void hbc::chunker::create_new_chunk() {

  if (file) {
  
    close_chunk();
    file_id++;
  }
  
  char buffer[strlen(directory) + 1 + num_digits(file_id) + 1];
  sprintf(buffer, "%s/%i%c", directory, file_id, 0);

  file = fopen(buffer, "w");
  size = 0;
  
  if (on_create_new_chunk) {
  
    on_create_new_chunk(this);
  }
}

void hbc::chunker::close_chunk() {

  fclose(file);
  file = 0;
}

void hbc::chunker::cleanup() {

  close_chunk();
  
  hbc::curler::cleanup();
}

void hbc::chunker::init() {

  hbc::curler::init();
  
  curl_easy_setopt(get_curl(), CURLOPT_WRITEFUNCTION, &hbc::chunk_write);
  curl_easy_setopt(get_curl(), CURLOPT_WRITEDATA, this);
  
  mkdir(directory, 0757);
  
  create_new_chunk();
}

hbc::chunker::chunker(const char* url, const char* directory, int chunk_size) : hbc::curler(url) {

  file_id = 0;
  size = 0;
  file = 0;
  
  this->directory = directory;
  this->chunk_size = chunk_size;
  
  on_create_new_chunk = 0;
  on_close_chunk = 0;
}

hbc::chunker::~chunker() {

  if (directory) {
  
    delete directory;
    directory = 0;
  }
  
  on_close_chunk = 0;
  on_create_new_chunk = 0;
}
