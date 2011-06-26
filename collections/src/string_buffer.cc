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

#include "string_buffer.h"

/*
 *
 */
hbcn_collections::string_buffer::string_buffer() {

  // do nothing
}

/*
 *
 */
hbcn_collections::string_buffer::~string_buffer() {

  // do nothing
}

/**
 * 
 */
char* hbcn_collections::string_buffer::to_string() {

  return to_string(0, m_buffer.size());
}

/**
 * 
 */
char* hbcn_collections::string_buffer::to_string(int start, int end) {

  char* buffer = (char*)malloc((end-start+1)*sizeof(char));
  buffer[end-start] = 0;
  
  for (int i = start; i < end; i++) {
  
    buffer[i-start] = m_buffer[i];
  }
  
  return buffer;
}

/**
 * 
 */
void hbcn_collections::string_buffer::trim() {

  while (!m_buffer.empty() && m_buffer.first() <= ' ') {
  
    m_buffer.pop_front();
  }
  
  while (!m_buffer.empty() && m_buffer.last() <= ' ') {
  
    m_buffer.pop_back();
  }
}

/**
 * 
 */
void hbcn_collections::string_buffer::append(const unsigned char& c) {

  m_buffer.push_back(c);
}

/**
 * 
 */
void hbcn_collections::string_buffer::append(const char& c) {

  append((unsigned char)(c & 0xFF));
}

/**
 * 
 */
void hbcn_collections::string_buffer::append(const unsigned char* c, int length) {

  for (int i = 0; i < length; i++) {
  
    append(c[i]);
  }
}

/**
 * 
 */
void hbcn_collections::string_buffer::append(const char* c, int length) {

  for (int i = 0; i < length; i++) {
  
    append(c[i]);
  }
}

/**
 * 
 */
void hbcn_collections::string_buffer::prepend(const unsigned char& c) {

  m_buffer.push_front(c);
}

/**
 * 
 */
void hbcn_collections::string_buffer::prepend(const char& c) {

  prepend((unsigned char)(c & 0xFF));
}

/**
 * 
 */
void hbcn_collections::string_buffer::prepend(const unsigned char* c, int length) {

  for (int i = length-1; i >= 0; i--) {
  
    prepend(c[i]);
  }
}

/**
 * 
 */
void hbcn_collections::string_buffer::prepend(const char* c, int length) {

  for (int i = length-1; i >= 0; i--) {
  
    prepend(c[i]);
  }
}

/**
 * 
 */
const unsigned char hbcn_collections::string_buffer::operator[](const int index) {

  return m_buffer[index];
}

/**
 * 
 */
void hbcn_collections::string_buffer::pop_front() {

  m_buffer.pop_front();
}

/**
 * 
 */
void hbcn_collections::string_buffer::pop_back() {

  m_buffer.pop_back();
}

/**
 * 
 */
int hbcn_collections::string_buffer::size() {

  m_buffer.size();
}
