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

#ifndef HBC_BASE64_H
#define HBC_BASE64_H

namespace hbc {

  int encoded_length(int decoded_length);
  int decoded_length(int encoded_length);
  
  void encode(const unsigned char* decoded, char* encoded, int decoded_length);
  void decode(const char* encoded, unsigned char* decoded, int encoded_length);
}

#endif
