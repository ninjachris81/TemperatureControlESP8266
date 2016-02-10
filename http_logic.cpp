#include "http_logic.h"
#include "debug.h"

void HttpLogic::init() {
  Debug::debugMsg("INIT HTTP");

  for (uint8_t i=0;i<FIELD_COUNT;i++) {
    currentData[i] = 0;
  }
}

void HttpLogic::update() {
  if (lastUpdate==0 || millis() - lastUpdate >  HTTP_UPDATE_INTERVAL_MS) {
    if (updateHttp()) {
      lastUpdate = millis();
    } else {
      lastUpdate = millis() - HTTP_UPDATE_INTERVAL_MS + HTTP_RETRY_MS;
      Debug::debugMsg("HTTP FAILED, Retry in", HTTP_RETRY_MS);
    }
  }
}

void HttpLogic::updateFieldValue(uint8_t index, int value) {
  if (index>=FIELD_COUNT || index < 0) return;
  currentData[index] = value;
}

bool HttpLogic::updateHttp() {
  HTTPClient http;
  http.setUserAgent("TempControl");

  String query = "/update?api_key=CZPKJGC87RGLCY6J";
  for (uint8_t i=0;i<FIELD_COUNT;i++) {
    addParam(query, i+1, currentData[i]);
  }

  Debug::debugMsg(query.c_str());
  
  http.begin("184.106.153.149", 80, query); //HTTP

  uint8_t timeout = 0;
  while(!http.connected()) {
    delay(250);
    timeout++;
    if (timeout>20) break;
  }

  // start connection and send HTTP header
  int httpCode = http.GET();
  Debug::debugMsg("GET RC:", httpCode);

  // httpCode will be negative on error
  if(httpCode > 0) {
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
        OUTPUT_SERIAL.println("HTTP OK");
        return true;
      }
  }

  http.end();
  
  return false;
}

void HttpLogic::addParam(String &query, uint8_t index, int value) {
  query+=F("&field");
  query+=index;
  query+=F("=");
  query+=value;
}

