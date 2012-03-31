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

#ifndef HBC_HTTP_REQUEST_H
#define HBC_HTTP_REQUEST_H

#include <map>
#include <string>
#include <vector>

#include <curl/curl.h>

#define HTTP_POST "POST"
#define HTTP_GET "GET"

namespace hbc {

  class http_request {
  
    public:

      std::string method;
      std::string url;
      std::string content_type;
      std::vector<unsigned char> post_data;
      std::map<std::string, std::string> headers;

      CURL* get_curl();
  };
}

#endif
