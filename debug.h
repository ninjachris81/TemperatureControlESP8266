#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "globals.h"

#define DEBUG_SERIAL OUTPUT_SERIAL

#ifdef IS_DEBUG
  #define OUTPUT_DEBUG true
#else
  #define OUTPUT_DEBUG false
#endif

class Debug {
public:
  static void setDebug(bool isDebug);  

  static void debugMsg(const char* msg);
  static void debugMsg(const char* msg, bool param1);
  static void debugMsg(const char* msg, int param1);
  static void debugMsg(const char* msg, unsigned int param1);
  static void debugMsg(const char* msg, unsigned int param1, unsigned int param2);
  static void debugMsg(const char* msg, String param1);

  static bool isDebug;

};

#endif
