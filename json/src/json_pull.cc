/*
 * Copyright (c) 2010-2012 Martin M Reed
 * Copyright (c) 2008-2010 Metova, Inc.
 * Copyright (c) 2007 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "json_pull.h"
#include "json_syntax.h"

#include <string.h>

hbc::json_enumeration* hbc::json_pull::object_elements() {

  if (next(D_START_OBJECT)) {
  
    return new json_enumeration(this, get_level(), D_END_OBJECT);
  }
  else {
  
    return new json_enumeration(this, get_level());
  }
}

hbc::json_enumeration* hbc::json_pull::array_elements() {

  if (next(D_START_ARRAY)) {
  
    return new json_enumeration(this, get_level(), D_END_ARRAY);
  }
  else {
  
    return new json_enumeration(this, get_level());
  }
}

int hbc::json_pull::next(int assertEvent) {

  int event = next();
  assert_event_type(event, assertEvent);
  return !(event == D_NULL && (assertEvent == D_START_ARRAY || assertEvent == D_START_OBJECT));
}

void hbc::json_pull::assert_event_type(int actual, int expected) {

  if (actual != D_NULL && actual != expected) {
  
    throw -1;// "expected " + (char) expected + ", found " + (char) actual + ' ' + string_value
  }
}

int hbc::json_pull::seek_in_object(const char* key) {

  json_enumeration* enumerator = object_elements();
  
  while (enumerator->has_more_elements()) {
  
    if (strcmp(enumerator->next_element(), key) == 0) {
    
      return 1;
    }
  }
  
  free(enumerator);
  
  return 0;
}

char* hbc::json_pull::try_get_string_value(const char* key) {

  return seek_in_object(key) ? get_string_value() : 0;
}

const char* hbc::json_pull::get_key() {

  next(D_KEY);
  return string_value;
}

char* hbc::json_pull::get_string_value() {

  next(D_STRING);
  return string_value;
}

char* hbc::json_pull::get_escaped_string_value() {

  next(D_STRING);
  return string_value;
}

char* hbc::json_pull::get_variable_value() {

  next(D_VARIABLE);
  return string_value;
}

int hbc::json_pull::get_boolean_value() {

  next();
  return strcmp("true", string_value) == 0;
}

int hbc::json_pull::get_integer_value() {

  next();
  return atoi(string_value);
}

double hbc::json_pull::get_double_value() {

  next();
  return (double)atof(string_value);
}

long hbc::json_pull::get_long_value() {

  next();
  return atol(string_value);
}

long hbc::json_pull::get_date_value() {

  char* text = get_string_value();
  int start = strstr(text, "\\/Date(") - text;
  int end = strstr(text, ")\\/") - text;
  
  char buffer[end-start+1];
  buffer[end-start] = 0;
  
  for (int i = start; i < end; i++) {
  
    buffer[i-start] = text[i];
  }
  
  return atol(buffer);
}
