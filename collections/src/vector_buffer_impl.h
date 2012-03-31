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

template<class T> hbc::vector_buffer<T>::vector_buffer() {

  // do nothing
}

template<class T> hbc::vector_buffer<T>::~vector_buffer() {

  // do nothing
}

template<class T> void hbc::vector_buffer<T>::push_back(const T& object) {

  buffer.push_back(object);
}

template<class T> void hbc::vector_buffer<T>::push_front(const T& object) {

  buffer.insert(buffer.begin(), object);
}

template<class T> const T& hbc::vector_buffer<T>::operator[](const int index) {

  return buffer[index];
}

template<class T> void hbc::vector_buffer<T>::pop_front() {

  remove(0);
}

template<class T> void hbc::vector_buffer<T>::pop_back() {

  buffer.pop_back();
}

template<class T> void hbc::vector_buffer<T>::remove(int index) {

  buffer.erase(buffer.begin()+index);
}
