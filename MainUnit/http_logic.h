#ifndef HTTP_LOGIC_H
#define HTTP_LOGIC_H

#include "globals.h"

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "password.h"

#ifdef IS_DEBUG
  #define HTTP_UPDATE_INTERVAL_MS 20000    // 20 sec
  #define HTTP_CHECK_CMD_INTERVAL_MS 5000    // 5 sec
  #define HTTP_UPDATE_RETRY_MS 10000   // 10 sec
#else
  #define HTTP_UPDATE_INTERVAL_MS 300000    // 5 min
  #define HTTP_CHECK_CMD_INTERVAL_MS 10000    // 10 sec
  #define HTTP_UPDATE_RETRY_MS 30000   // 30 sec
#endif

#define HTTP_CODE_OK 200
#define HTTP_CODE_NOT_MODIFIED 304

#define FIELD_INDEX_WATER 0
#define FIELD_INDEX_HC 1
#define FIELD_INDEX_TANK 2
#define FIELD_INDEX_PUMP_W 3
#define FIELD_INDEX_PUMP_HC 4
#define FIELD_INDEX_FLOW_SWITCH 5
#define FIELD_INDEX_FREE_RAM 6
#define FIELD_INDEX_RESTARTED 7

#define FIELD_COUNT 8

class HttpLogic {
public:

  void init();

  void update();

  void updateFieldValue(uint8_t index, int value);

  void setActive(bool isActive);

  bool updateHttp();

  bool checkHttpCmd();

private:
  long lastUpdateHttp = 0;
  long lastCheckCmd = 0;

  bool isActive = false;

  int currentData[8];

  void addParam(String &query, uint8_t index, int value);

};

#endif
