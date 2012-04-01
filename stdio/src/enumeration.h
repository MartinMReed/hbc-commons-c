/*
 * Copyright (c) 2010 Martin M Reed
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

#ifndef HBC_ENUMERATION_H
#define HBC_ENUMERATION_H

#include <stdlib.h>
using namespace std;

namespace hbc {

  template<class T> class enumeration {
  
    public:
    
    virtual int has_more_elements() = 0;
    virtual T next_element() = 0;
  };
}

#endif
