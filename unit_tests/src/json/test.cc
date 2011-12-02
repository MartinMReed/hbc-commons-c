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

#include "../libhbc_stdio/file_input_stream.h"
using namespace hbcn_stdio;

#include "../libhbc_json/json_pull.h"
using namespace hbcn_json;

#include <string.h>

void parse_objects(json_pull& parser, int level);
void parse_feed(json_pull& parser, int level);

int json_test() {
  
  file_input_stream input_stream("json/watch_on_mobile.json");
  json_pull parser(&input_stream);
  parse_objects(parser, 0);
  
  return 0;
}

void parse_objects(json_pull& parser, int level) {

  json_enumeration* enumerator = parser.object_elements();
  
  while (enumerator->has_more_elements()) {
  
    const char* key = enumerator->next_element();
    
    if (strcmp(key, "feed") == 0) {
    
      parse_feed(parser, 1);
    }
    else if (strcmp(key, "version") == 0) {
    
      printf("version[%s]\n", parser.get_string_value());
    }
    else if (strcmp(key, "encoding") == 0) {
    
      printf("encoding[%s]\n", parser.get_string_value());
    }
  }
  
  free(enumerator);
}

void getT(json_pull& parser) {

  json_enumeration* enumerator = parser.object_elements();
  
  while (enumerator->has_more_elements()) {
  
    if (strcmp(enumerator->next_element(), "$t") == 0) {
    
      printf("%s", parser.get_string_value());
    }
  }
  
  free(enumerator);
}

void parse_feed(json_pull& parser, int level) {

  json_enumeration* enumerator = parser.object_elements();
  
  while (enumerator->has_more_elements()) {
  
    const char* key = enumerator->next_element();
    
    if (strcmp(key, "id") == 0) {
    
      printf("--id[");
      getT(parser);
      printf("]\n");
    }
    else if (strcmp(key, "title") == 0) {
    
      printf("--title[");
      getT(parser);
      printf("]\n");
    }
    else if (strcmp(key, "logo") == 0) {
    
      printf("--logo[");
      getT(parser);
      printf("]\n");
    }
    else if (strcmp(key, "openSearch$itemsPerPage") == 0) {
    
      printf("--openSearch$itemsPerPage[");
      getT(parser);
      printf("]\n");
    }
    else {
    
      printf("--key[%s]\n", key);
    }
  }
  
  free(enumerator);
}
