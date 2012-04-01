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

#ifndef HBC_JSON_ENUMERATION_H
#define HBC_JSON_ENUMERATION_H

#include "libhbc_stdio/enumeration.h"

namespace hbc {

  class json_pull;
  
  class json_enumeration : public enumeration<const char*> {
  
    friend class json_pull;
    
    public:
    
    int has_more_elements();
    const char* next_element();
    
    private:
    
    json_enumeration(json_pull* parser, int level, int terminator = -1);
    
    json_pull* parser;
    int terminator;
    int level;
  };
}

#endif
