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

#ifndef HBC_CURL_H
#define HBC_CURL_H

#include <curl/curl.h>

namespace hbc {

  class curler {
  
    public:
    
    curler(const char* url);
    ~curler();
    
    int download();
    
    void (*on_init)(void* stream);
    void (*on_perform_start)(void* stream);
    void (*on_perform_end)(void* stream);
    void (*on_cleanup)(void* stream);
    
    CURL* get_curl() { return curl; }
    const char* get_url() { return url; }
    
    protected:
    
    virtual void init();
    virtual void perform();
    virtual void cleanup();
    
    private:
    
    CURL* curl;
    const char* url;
  };
}

#endif
