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
  #define HTTP_FLASH_CHECK_INTERVAL_MS 10000
  #define SERVER_IP "192.168.178.24"
#else
  #define HTTP_FLASH_CHECK_INTERVAL_MS 60000
  #define SERVER_IP "192.168.178.49"
#endif

class HttpLogic {
public:
  void init();

  void update();

  void postCommand(String cmd);

  void checkChannel(bool forceCheck=false);

  void checkFlash();

private:
  long lastFlashCheck = 0;

  WiFiClient client;

  int lastEntryId = 0;

  String lastData = "";

  String getFieldValue(String data, uint8_t index);
  
};


#endif
