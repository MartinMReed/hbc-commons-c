/*
   Test program for TinyXML.
*/
#ifndef TINYXML_TEST_H
#define TINYXML_TEST_H

#include "../../tinyxml/tinyxml/tinyxml.h"

#ifdef TIXML_USE_STL
  #include <iostream>
  #include <sstream>
  using namespace std;
#else
  #include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
  #include <crtdbg.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
  _CrtMemState startMemState;
  _CrtMemState endMemState;
#endif

static int gPass = 0;
static int gFail = 0;

int tinyxml_test();
bool XmlTest (const char* testString, const char* expected, const char* found, bool noEcho = false);
bool XmlTest( const char* testString, int expected, int found, bool noEcho = false );

#endif
