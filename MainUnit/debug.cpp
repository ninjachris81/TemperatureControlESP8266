#include "debug.h"

bool Debug::isDebug = OUTPUT_DEBUG;

void Debug::setDebug(bool isDebug) {
  Debug::isDebug = isDebug;
}

void Debug::debugMsg(const char* msg) {
  if (!isDebug) return;
  DEBUG_SERIAL.println(msg);
}

void Debug::debugMsg(const char* msg, bool param1) {
  if (!isDebug) return;
  DEBUG_SERIAL.print(msg);
  DEBUG_SERIAL.print(" ");
  DEBUG_SERIAL.println(param1);
}

void Debug::debugMsg(const char* msg, int param1) {
  if (!isDebug) return;
  DEBUG_SERIAL.print(msg);
  DEBUG_SERIAL.print(" ");
  DEBUG_SERIAL.println(param1);
}

void Debug::debugMsg(const char* msg, unsigned int param1) {
  if (!isDebug) return;
  DEBUG_SERIAL.print(msg);
  DEBUG_SERIAL.print(" ");
  DEBUG_SERIAL.println(param1);
}

void Debug::debugMsg(const char* msg, unsigned int param1, unsigned int param2) {
  if (!isDebug) return;
  DEBUG_SERIAL.print(msg);
  DEBUG_SERIAL.print(" ");
  DEBUG_SERIAL.print(param1);
  DEBUG_SERIAL.print(" ");
  DEBUG_SERIAL.println(param2);
}

void Debug::debugMsg(const char* msg, String param1) {
  if (!isDebug) return;
  DEBUG_SERIAL.print(msg);
  DEBUG_SERIAL.print(" ");
  DEBUG_SERIAL.println(param1);
}
