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

#include <math.h>
#include <sys/stat.h>
#include <string.h>

#include "libhbc_math/math.h"

#include "curl.h"
using namespace hbc;

/*
 *
 */
hbc::curler::curler(const char* url) {

  this->url = url;
  
  curl = 0;
  
  on_init = 0;
  on_perform_start = 0;
  on_perform_end = 0;
  on_cleanup = 0;
}

/*
 *
 */
hbc::curler::~curler() {

  on_init = 0;
  on_perform_start = 0;
  on_perform_end = 0;
  on_cleanup = 0;
  
  curl = 0;
}

/*
 *
 */
void hbc::curler::init() {

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
}

/*
 *
 */
void hbc::curler::perform() {

  curl_easy_perform(curl);
}

/*
 *
 */
void hbc::curler::cleanup() {

  curl_easy_cleanup(curl);
}

/*
 *
 */
int hbc::curler::download() {

  if (curl = curl_easy_init()) {
  
    init();
    
    if (on_init) {
    
      on_init(this);
    }
    
    if (on_perform_start) {
    
      on_perform_start(this);
    }
    
    try {
    
      perform();
    }
    catch(...) {
    
      // do something
    }
    
    if (on_perform_end) {
    
      on_perform_end(this);
    }
    
    cleanup();
    
    if (on_cleanup) {
    
      on_cleanup(this);
    }
    
    return 1;
  }
  
  return 0;
}
