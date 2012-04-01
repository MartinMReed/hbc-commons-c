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

#include "base/unit_base.h"

#include "thread/test.h"
#include "tinyxml/test.h"
#include "curl_input_stream/test.h"
#include "file_input_stream/test.h"
#include "json/test.h"

int main() {
  
  add_test("thread_test", &thread_test);
  add_test("tinyxml_test", &tinyxml_test);
  add_test("file_input_stream_test", &file_input_stream_test);
//  add_test("json_test", &json_test);
  add_test("curl_input_stream_test", &curl_input_stream_test, 1);
  
  return run_tests();
}
