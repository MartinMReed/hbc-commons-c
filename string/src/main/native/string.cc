/**
 * Copyright (c) 2009-2010 Martin M Reed
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

/**
 * 
 */
char* hbcn_string::memtrim(char* text) {

  int text_len = strlen(text);
  
  char* _text = text;
  
  text = (char*) calloc(text_len + 1, sizeof(char));
  
  memcpy(text, _text, text_len - 1 * sizeof(char));
  free(_text);
  
  return text;
}

/**
 * 
 */
int hbcn_string::is_whitespace(char c) {

  switch (c) {
  
    case WHITESPACE:
    case NEWLINE:
    case CARRIAGE_RETURN:
    case EOF:
      return 1;
      
    default:
      return 0;
  }
}

/**
 * 
 */
int hbcn_string::is_newline(char c) {
  
  switch (c) {
  
    case NEWLINE:
    case CARRIAGE_RETURN:
      return 1;
      
    default:
      return 0;
  }
}

/**
 * Create a copy and then perform an in-place trim of the provided string.
 * 
 * @param text
 * @return
 */
char* hbcn_string::trim(char* text) {

  int text_len = strlen(text);
  char* temp = (char*) calloc(text_len + 1, sizeof(char));
  temp[text_len] = 0;
  
  for (int i = 0; i < text_len; i++) {
  
    temp[i] = text[i];
  }
  
  return trim_inplace(temp);
}

/**
 * Perform an in-place trim of the provided string.
 * 
 * @param text
 * @return
 */
char* hbcn_string::trim_inplace(char* text) {

  int text_len = strlen(text);
  
  // first we will trim the ending since
  // no characters have to be shifted in-place
  // and we are just shortening the text length
  for (int i = text_len - 1; i >= 0; i--) {
  
    if (is_whitespace(text[i])) {
    
      text[i] = 0;
    }
    else {
    
      text_len = i + 1;
      break;
    }
  }
  
  // i = position within the original text
  // j = position within the new text
  // k = amount of whitespace at the start
  for (int i = 0, j = 0, k = 0; i < text_len + k; i++) {
  
    if (j == 0 && is_whitespace(text[i])) {
    
      k++;
      continue;
    }
    
    text[j++] = (i < text_len) ? text[i] : 0;
  }
  
  return text;
}


/**
 * 
 */
char** hbcn_string::split(char* text, char c) {

  int result_len = 1;
  char** result = (char**) calloc(result_len, sizeof(char*));
  
  bool found_marker = true;
  
  for (int i = 0; text[i]; i++) {
  
    if (text[i] == c) {
    
      found_marker = true;
    }
    else {
    
      // found a new word so grow the array
      if (found_marker) {
      
        found_marker = false;
        
        char** _result = result;
        int _result_len = result_len;
        
        result = (char**) calloc(++result_len, sizeof(char*));
        
        memcpy(result, _result, _result_len - 1 * sizeof(char*));
        free(_result);
      }
      
      // grow the string before appending the next character
      
      int index = result_len - 2;
      
      char* word = result[index];
      int word_len = word ? strlen(result[index]) : 0;
      
      result[index] = (char*) calloc(word_len + 2, sizeof(char));
      
      if (word) {
      
        memcpy(result[index], word, word_len * sizeof(char));
        free(word);
      }
      
      // append the next character
      result[index][word_len] = text[i];
    }
  }
  
  return result;
}
