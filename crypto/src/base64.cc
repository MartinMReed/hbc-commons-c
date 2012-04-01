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

#include "base64.h"

#define ENCODER "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define PADDING '='

namespace hbc {

  void encode_block(unsigned char in[3], char out[4], int len) {
  
    out[0] = ENCODER[in[0] >> 2];
    out[1] = ENCODER[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
    out[2] = (len > 1) ? ENCODER[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : PADDING;
    out[3] = (len > 2) ? ENCODER[in[2] & 0x3f] : PADDING;
  }
  
  void decode_block(char in[4], unsigned char out[3]) {
  
    out[0] = (in[0] << 0x02) | ((in[1] & 0x30) >> 4);
    out[1] = ((in[1] & 0x0f) << 4) | ((in[2] & 0x3c) >> 2);
    out[2] = ((in[2] & 0x03) << 6) | in[3];
  }
  
  unsigned char decode_value(const char in) {
  
    if (in == PADDING) {
    
      return 0;
    }
    
    for (int i = 0; i < 64; i++) {
    
      if (ENCODER[i] == in) {
      
        return i;
      }
    }
    
    return 0;
  }
}

int hbc::encoded_length(int length) {

  return (((length+2)-((length+2)%3))/3)*4;
}

int hbc::decoded_length(int length) {

  return length*(3/4);
}

void hbc::encode(const unsigned char* decoded, char* encoded, int decoded_length) {

  unsigned char in[3];
  char out[4];
  
  for (int i = 0, j = 0; i < decoded_length; i += 3, j += 4) {
  
    // do not access the input string
    // using the array accessing style
    // because we may be at the end and
    // get a null pointer
    in[0] = *(decoded+i+0);
    in[1] = *(decoded+i+1);
    in[2] = *(decoded+i+2);
    
    // length is sent incase we have gone
    // past the end of our input string
    encode_block(in, out, decoded_length - i);
    
    encoded[j+0] = out[0];
    encoded[j+1] = out[1];
    encoded[j+2] = out[2];
    encoded[j+3] = out[3];
  }
}

void hbc::decode(const char* encoded, unsigned char* decoded, int encoded_length) {

  char in[4];
  unsigned char out[3];
  
  for (int i = 0, j = 0; i < encoded_length; i += 4, j += 3) {
  
    in[0] = decode_value(encoded[i+0]);
    in[1] = decode_value(encoded[i+1]);
    in[2] = decode_value(encoded[i+2]);
    in[3] = decode_value(encoded[i+3]);
    
    decode_block(in, out);
    
    decoded[j+0] = out[0];
    decoded[j+1] = out[1];
    decoded[j+2] = out[2];
  }
}
