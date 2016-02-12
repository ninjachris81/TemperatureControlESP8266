#ifndef HTTP_LOGIC_H
#define HTTP_LOGIC_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define HTTP_CHECK_INTERVAL_MS 20000

class HttpLogic {
public:
  void init();

  void update();

  bool postCommand(String cmd);

  void syncData();
  
private:
  unsigned long lastCheck = 0;
};


#endif
