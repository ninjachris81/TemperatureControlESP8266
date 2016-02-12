#ifndef HTTP_LOGIC_H
#define HTTP_LOGIC_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "globals.h"

#ifdef IS_DEBUG
  #define HTTP_CHECK_INTERVAL_MS 5000
#else
  #define HTTP_CHECK_INTERVAL_MS 10000
#endif

class HttpLogic {
public:
  void init();

  void update();

  bool postCommand(String cmd);

  void checkData();
  
private:
  long lastCheck = 0;

  String lastData = "";
  
};


#endif
