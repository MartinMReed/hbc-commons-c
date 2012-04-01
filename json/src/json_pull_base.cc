/*
 * Copyright (c) 2010-2012 Martin M Reed
 * Copyright (c) 2008-2010 Metova Inc.
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

hbc::json_pull::json_pull(input_stream* input_stream) {

  m_input_stream = input_stream;
  
  string_value = 0;
  
  pos = 0;
  end = 0;
  
  state = STATE_END;
  push_and_set_state(STATE_VALUE);
}

hbc::json_pull::~json_pull() {

  pos = 0;
  end = 0;
  
  state = 0;
  
  if (string_value) {
  
    free(string_value);
    string_value = 0;
  }
}

int hbc::json_pull::read() {

  unsigned char buffer[1];
  m_input_stream->read(buffer, 1);
  m_buffer.push_back(buffer[0]);
  
  pos++;
  
  return buffer[0];
}

char hbc::json_pull::next_value() {

  char read = next_buffer_value();

  // remove all whitespace
  while (read <= ' ') {

    m_buffer.pop_front();
    read = next_buffer_value();
  }

  return read;
}

char hbc::json_pull::next_buffer_value() {

  if (m_buffer.empty()) {

    read();
  }

  return m_buffer.front();
}

int hbc::json_pull::return_value(int value, int state) {

  m_buffer.pop_front();
  this->state = state;
  return value;
}

int hbc::json_pull::next() {

  char next = next_value();

  //printf("state[%i], next[%c], string_value[%s]\n", state, next, string_value ? string_value : "null");

  switch (state) {

    case STATE_END:
      throw -1;// "Called next() after the end";

    case STATE_VALUE:
      return value_state(next);

    case STATE_ARRAY:
      return array_state(next);

    case STATE_OBJECT:
      return object_state(next);
  }

  throw -2;//"lexical error: " + next;
}

int hbc::json_pull::value_state(char next) {

  switch (next) {

    case D_QUOTE: {

      parse_and_set_string();
      pop_state();
      
      // push a dummy character
      m_buffer.push_front((char)-1);
      
      return return_value(D_STRING, state);
    }

    case D_START_OBJECT:
      return return_value(D_START_OBJECT, STATE_OBJECT);

    case D_START_ARRAY:
      return return_value(D_START_ARRAY, STATE_ARRAY);

    default:
      return default_value_state(next);
  }
}

int hbc::json_pull::default_value_state(char next) {

  parse_and_set_variable();
  pop_state();

  if (null_or_empty(string_value)) {

    if (string_value) {
    
      free(string_value);
      string_value = 0;
    }
    
    return return_value(D_NULL, state);
  }

  return return_value(D_VARIABLE, state);
}

int hbc::json_pull::array_state(char next) {

  if (next == D_SEPERATOR) {

    m_buffer.pop_front();
    next = next_value();
  }

  switch (next) {

    case D_END_ARRAY: {

      pop_state();
      return return_value(D_END_ARRAY, state);
    }

    case D_QUOTE: {

      parse_and_set_string();
      return return_value(D_STRING, state);
    }

    case D_START_OBJECT: {

      push_and_set_state(STATE_OBJECT);
      return return_value(D_START_OBJECT, state);
    }

    case D_START_ARRAY: {

      push_and_set_state(STATE_ARRAY);
      return return_value(D_START_ARRAY, state);
    }

    default:
      return default_array_state(next);
  }
}

int hbc::json_pull::default_array_state(char next) {

  parse_and_set_variable();

  if (null_or_empty(string_value)) {

    if (string_value) {
    
      free(string_value);
      string_value = 0;
    }
    
    return return_value(D_NULL, state);
  }

  return return_value(D_VARIABLE, state);
}

int hbc::json_pull::object_state(char next) {

  if (next == D_SEPERATOR) {

    m_buffer.pop_front();
    next = next_value();
  }

  switch (next) {

    case D_END_OBJECT: {

      pop_state();
      return return_value(D_END_OBJECT, state);
    }

    case D_QUOTE: {

      parse_and_set_string();

      next = next_value();

      if (next != ':') {

        throw -3;//"Expected ':', found '" + next + "' at pos[" + pos + "]";
      }

      push_and_set_state(STATE_VALUE);
      return return_value(D_KEY, state);
    }

    default:
      throw -4;//"Unexpected '" + next + "' in object at pos[" + pos + "]";
  }
}

int hbc::json_pull::null_or_empty(char* text) {

  return !text || strlen(text) == 0 || strcmp(text, "null") == 0;
}

bool is_whitespace(unsigned char c) {

  return strstr(" \t\n\r", (char*)&c) >= 0;
}

void hbc::json_pull::set_string(int trim_whitespace) {

  if (trim_whitespace) {
  
    while (!m_buffer.empty() && is_whitespace(m_buffer.front())) {
      m_buffer.pop_front();
    }

    while (!m_buffer.empty() && is_whitespace(m_buffer.back())) {
      m_buffer.pop_back();
    }
  }
  
  if (string_value) {
  
    free(string_value);
  }
  
  string_value = new char[end + 1];
  string_value[end-1] = NULL;

  for (int i = 0; i < end; i++) {
    string_value[i] = (char)m_buffer[i];
  }
  
  for (int i = 0; i < end; i++) {
  
    m_buffer.pop_front();
  }
}

void hbc::json_pull::seek_level(int level) {

  while (get_level() > level) {

    next();
  }
}

int hbc::json_pull::get_level() {

  return state_stack.size();
}

void hbc::json_pull::parse_and_set_string() {

  // remove the starting " character
  m_buffer.pop_front();

  end = index_of(PARSE_OPTIONS_STRING)-1;

  // remove the ending " character
  m_buffer.pop_back();

  set_string(0);
}

void hbc::json_pull::parse_and_set_variable() {

  end = index_of(PARSE_OPTIONS_VARIABLE);

  set_string(1);

  // push a dummy character
  m_buffer.push_front((char)-1);
}

int hbc::json_pull::index_of(char* find) {

  int previous[2];
  previous[0] = -1;
  previous[1] = -1;

  for (int _read = read(), i = 1; _read > -1; _read = read(), i++) {

    // if previous is not \, the escape char, we should check if we've found a match. If previous previous is a \, we should also check
    // because we could be seeing an escaped \ - which would be a \\.
    if (previous[0] != '\\' || previous[1] == '\\') {
      
      // check if the current character is in the given set
      for (int j = 0; j < strlen(find); j++) {
      
        if (find[j] == _read) {
        
          return i;
        }
      }
    }

    previous[1] = previous[0];
    previous[0] = _read;
  }

  throw -5;//"missing closing characters";
}

void hbc::json_pull::pop_state() {

  state = state_stack.back();
  state_stack.pop_back();
}

void hbc::json_pull::push_and_set_state(int newState) {

  state_stack.push_back(state);
  state = newState;
}
