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

#include "../libhbc_collections/bounded_buffer.h"
#include "../libhbc_collections/vector_buffer.h"
using namespace hbcn_collections;

#include <stdio.h>
#include <string.h>

void print_buffer(buffer<char>& buffer, const char* expected) {

  int buffer_size = buffer.size();
  char* actual = new char[buffer_size+1];
  actual[buffer_size] = 0;
  
  for (int i = 0; i < buffer_size; i++) {
  
    actual[i] = buffer[i];
  }
  
  printf("actual[%i, %s], expected[%i, %s]\n", buffer_size, actual, strlen(expected), expected);
  
  if (buffer_size != strlen(expected)) {
  
    throw -2;
  }
  
  if (strcmp(actual, expected) != 0) {
  
    throw -1;
  }
}

void test_buffer(buffer<char>& buffer) {
  
  const char C0 = '0';
  const char C3 = '3';
  char C5 = '5';
  char C9 = '9';
  
  buffer.push_front('2');
  buffer.push_back(C3);
  buffer.push_back('4');
  buffer.push_front('1');
  buffer.push_back(C5);
  buffer.push_back('6');
  buffer.push_back('7');
  buffer.push_back('8');
  buffer.push_back(C9);
  buffer.push_front(C0);
  print_buffer(buffer, "0123456789");
  
  buffer.remove(1); 
  print_buffer(buffer, "023456789");
  
  buffer.pop_front(); 
  print_buffer(buffer, "23456789");
  
  buffer.remove(2); 
  print_buffer(buffer, "2356789");
  
  buffer.remove(2); 
  print_buffer(buffer, "236789");
  
  buffer.pop_back(); 
  print_buffer(buffer, "23678");
  
  buffer.remove(3); 
  print_buffer(buffer, "2368");
}

int collections_test() {

  bounded_buffer<char> b_buffer(10);
  test_buffer(b_buffer);
  
  vector_buffer<char> v_buffer;
  test_buffer(v_buffer);
  
  return 0;
}