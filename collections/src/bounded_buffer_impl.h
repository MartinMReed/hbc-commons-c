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

template<class T> hbc::bounded_buffer<T>::bounded_buffer(int capacity) {

  this->m_capacity = m_capacity;
  m_size = 0;
  start_index = 0;
  
  m_buffer = new T[m_capacity];
}

template<class T> hbc::bounded_buffer<T>::~bounded_buffer() {

  m_capacity = 0;
  m_size = 0;
  start_index = 0;
  
  if (m_buffer) {
  
    delete[] m_buffer;
    m_buffer = 0;
  }
}

template<class T> inline void hbc::bounded_buffer<T>::increment_start_index() {

  int _start_index = start_index+1;
  start_index = _start_index == m_capacity ? 0 : _start_index;
}

template<class T> inline void hbc::bounded_buffer<T>::decrement_start_index() {

  int _start_index = start_index-1;
  start_index = _start_index == -1 ? m_capacity-1 : _start_index;
}

template<class T> inline int hbc::bounded_buffer<T>::translate(int index) {

  if (start_index == 0) {
  
    return index;
  }
  
  index += start_index;
  
  if (index < m_capacity) {
  
    return index;
  }
  
  return index-m_capacity;
}

template<class T> const T& hbc::bounded_buffer<T>::operator[](const int index) {

  return m_buffer[translate(index)];
}

template<class T> void hbc::bounded_buffer<T>::push_back(const T& object) {

  memcpy(m_buffer+translate(m_size), &object, sizeof(T));
  m_size++;
}

template<class T> void hbc::bounded_buffer<T>::push_front(const T& object) {

  decrement_start_index();
  memcpy(m_buffer+start_index, &object, sizeof(T));
  m_size++;
}

template<class T> void hbc::bounded_buffer<T>::pop_front() {

  remove(0);
}

template<class T> void hbc::bounded_buffer<T>::pop_back() {

  remove(m_size-1);
}

template<class T> void hbc::bounded_buffer<T>::remove(int index) {

  if (index == 0 || index < m_size*0.50) {
  
    if (index > 0) {
    
      // if we are removing from the front half,
      // then shift the elements forward
      
      for (; index > 0; index--) {
      
        m_buffer[translate(index)] = m_buffer[translate(index-1)];
      }
    }
    
    increment_start_index();
  }
  else {
  
    if (index < m_size-1) {
    
      // if we are removing from the back half,
      // then shift the elements backward
      
      for (; index < m_size-1; index++) {
      
        m_buffer[translate(index)] = m_buffer[translate(index+1)];
      }
    }
  }
  
  m_size--;
  
  if (m_size == 0) {
  
    // shift the start index back to zero if we are empty
    start_index = 0;
  }
}
