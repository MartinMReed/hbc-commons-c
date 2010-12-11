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

#include "unit_base.h"

#include <queue>
using namespace std;

queue<unit_runnable*> tests;

void add_test(char* name, unit_test test, int failable) {

  unit_runnable* runnable = new unit_runnable();
  runnable->name = name;
  runnable->test = test;
  runnable->failable = failable;
  tests.push(runnable);
}

int run_tests() {

  while (!tests.empty()) {
  
    unit_runnable* runnable = tests.front();
    tests.pop();
    
    int result;
    const char* error_message = "unknown reason";
    
    try {
    
      result = runnable->test();
    }
    catch (int i) {
    
      result = i;
    }
    catch (const char* e) {
    
      result = -1;
      error_message = e;
    }
    catch (...) {
    
      result = -1;
    }
    
    if (result) {
    
      printf("failed test[%s]: code[%i], reason[%s]%s\n", runnable->name, result, error_message, runnable->failable ? "... ignoring fail" : "");
      return result;
    }
    else {
    
      // clear the screen
      //printf("%c%s", 27, "[2J");
      //printf("%c%s", 27, "[1;1H");
    }
  }
  
  printf("all tests are successful\n");
  return 0;
}
