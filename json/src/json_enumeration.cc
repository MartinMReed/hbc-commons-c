/*
 * Copyright (c) 2010 Martin M Reed
 * Copyright (c) 2008-2010 Metova, Inc.
 * Copyright (c) 2007 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "json_enumeration.h"
#include "json_pull.h"

hbc::json_enumeration::json_enumeration(json_pull* parser, int level, int terminator) {

  this->parser = parser;
  this->level = level;
  this->terminator = terminator;
}

int hbc::json_enumeration::has_more_elements() {

  if (terminator != -1) {
  
    parser->seek_level(level);
    
    if (parser->next_value() != terminator) {
    
        return 1;
    }
    
    parser->next();
  }
  
  return 0;
}

const char* hbc::json_enumeration::next_element() {

  return parser->get_key();
}
