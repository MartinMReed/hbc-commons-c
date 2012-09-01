/**
 * Copyright (c) 2009-2012 Martin M Reed
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

#ifndef HBC_STRING_H
#define HBC_STRING_H

#define NEWLINE '\n'
#define CARRIAGE_RETURN '\r'
#define TAB '\t'
#define WHITESPACE ' '
#define POUND_SIGN '#'

namespace hbc {

  int is_whitespace(char c);
  int is_newline(char c);
  
  char** split(char* text, char c);
  
  char* memtrim(char* text);
  
  char* trim(char* text);
  char* trim_inplace(char* text);
  
  char** split(char* text, char c);
}

#endif
