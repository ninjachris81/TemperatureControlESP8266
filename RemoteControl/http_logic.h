#ifndef HTTP_LOGIC_H
#define HTTP_LOGIC_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "globals.h"

#define ENABLE_FLASH false

#ifdef IS_DEBUG
  #define HTTP_CHECK_INTERVAL_MS 5000
  #define HTTP_FLASH_CHECK_INTERVAL_MS 10000
#else
  #define HTTP_CHECK_INTERVAL_MS 10000
  #define HTTP_FLASH_CHECK_INTERVAL_MS 60000
#endif

class HttpLogic {
public:
  void init();

  void update();

  bool postCommand(String cmd);

  void checkData(bool forceCheck=false);

  void checkChannel(bool forceCheck=false);

  void checkFlash();

private:
  long lastCheck = 0;
  long lastFlashCheck = 0;

  int lastEntryId = 0;

  String lastData = "";

  String getFieldValue(String data, uint8_t index);
  
};


#endif
