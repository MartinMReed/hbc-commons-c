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

#ifndef HBC_JSON_SYNTAX_H
#define HBC_JSON_SYNTAX_H

#define D_START_OBJECT '{'
#define D_END_OBJECT '}'
#define D_START_ARRAY '['
#define D_END_ARRAY ']'
#define D_KEY 'k'
#define D_STRING 'a'
#define D_VARIABLE 'v'
#define D_SEPERATOR ','
#define D_NULL 'n'
#define D_QUOTE '"'

#define PARSE_OPTIONS_STRING "\""
#define PARSE_OPTIONS_VARIABLE ",]}"

#define STATE_END 1
#define STATE_VALUE 2
#define STATE_ARRAY 3
#define STATE_OBJECT 4

#endif
