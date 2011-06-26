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

#ifndef HBC_JSON_PULL_H
#define HBC_JSON_PULL_H

#include "libhbc_stdio/input_stream.h"
using namespace hbcn_stdio;

#include "libhbc_collections/enumeration.h"
#include "libhbc_collections/string_buffer.h"
using namespace hbcn_collections;

#include "json_enumeration.h"

namespace hbcn_json {

  class json_pull {
  
    friend class json_enumeration;
    
    public:
    
    json_pull(input_stream* input_stream);
    ~json_pull();
    
    /**
     * Must be called from inside an Object (e.g. right after reading D_START_OBJECT).
     */
    json_enumeration* object_elements();
    
    /**
     * Must be called from inside an Array (e.g. right after reading D_START_ARRAY).
     */
    json_enumeration* array_elements();
    
    /**
     * Skips forward inside an object until finds the given key.
     * Must be called from inside an object (e.g. right after reading D_START_OBJECT).
     * 
     * @param key
     * @returns true if the key was found, false otherwise.
     */
    int seek_in_object(const char* key);
    
    /**
     * Calls seekInObject(key). If the call is successful,
     * it returns getStringValue().
     * @param key
     * @return String value for given key if it exists, else null
     */
    char* try_get_string_value(const char* key);
    
    /**
     * Return the key as a String.
     * 
     * @return
     */
    const char* get_key();
    
    /**
     * Return the String value and unescape any Java
     * @return
     */
    char* get_string_value();
    
    /**
     * Return the original String value.
     * 
     * @return
     */
    char* get_escaped_string_value();
    
    /**
     * Return a variable String.
     * 
     * @return
     */
    char* get_variable_value();
    
    int get_boolean_value();
    
    /**
     * Return an int value.
     * Warning: If your Json response has any white space after the integer value you
     * are trying to obtain and the element is the last element of a block, this will fail.
     * 
     * @return
     */
    int get_integer_value();
    
    /**
     * Return a double value.
     * Warning: If your Json response has any white space after the double value you
     * are trying to obtain and the element is the last element of a block, this will fail.
     * @return
     */
    double get_double_value();
    
    /**
     * Return a long value.
     * Warning: If your Json response has any white space after the long value you
     * are trying to obtain and the element is the last element of a block, this will fail.
     * @return
     */
    long get_long_value();
    
    /**
     * Return a date value.
     * 
     * @return
     */
    long get_date_value();
    
    /** 
     * Advances to the next token, and returns its type.
     * @returns the token type, such as D_START_OBJECT, D_STRING, D_END_DOCUMENT.
     */
    int next();
    char next_value();
    
    private:
    
    int state;
    int pos;
    int end;
    
    input_stream* m_input_stream;
    vector_buffer<int> state_stack;
    string_buffer m_buffer;
    
    char* string_value;
    
    int next(int assert_event);
    void assert_event_type(int actual, int expected);
    
    /**
     * Advances to the next token on the given level.
     * Thus may advance through multiple tokens, until reaches the level.
     */
    void seek_level(int level);
    int get_level();
    
    int read();
    char next_buffer_value();
    int return_value(int value, int state);
    
    void push_and_set_state(int newState);
    void pop_state();
    
    int value_state(char next);
    int default_value_state(char next);
    int array_state(char next);
    int default_array_state(char next);
    int object_state(char next);
    
    void parse_and_set_string();
    void parse_and_set_variable();
    
    int index_of(char* find);
    
    int null_or_empty(char* text);
    void set_string(int trim_whitespace);
  };
}

#endif
