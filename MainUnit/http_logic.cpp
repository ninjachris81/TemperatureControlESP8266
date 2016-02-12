#include "http_logic.h"
#include "debug.h"
#include "http_utils.h"

void HttpLogic::init() {
  Debug::debugMsg("INIT HTTP");

  for (uint8_t i=0;i<FIELD_COUNT;i++) {
    currentData[i] = 0;
  }
}

void HttpLogic::update() {
  if (!this->isActive) return;
  
  if (lastUpdateHttp==0 || millis() - lastUpdateHttp >  HTTP_UPDATE_INTERVAL_MS) {
    if (updateHttp()) {
      lastUpdateHttp = millis();
    } else {
      lastUpdateHttp = millis() - HTTP_UPDATE_INTERVAL_MS + HTTP_UPDATE_RETRY_MS;
      Debug::debugMsg("HTTP FAILED, Retry in", HTTP_UPDATE_RETRY_MS);
    }
  }

  if (lastCheckCmd==0 || millis() - lastCheckCmd >  HTTP_CHECK_CMD_INTERVAL_MS) {
    if (checkHttpCmd()) {
      lastCheckCmd = millis();
    } else {
      Debug::debugMsg("Check Cmd failed");
    }
  }
}

void HttpLogic::setActive(bool isActive) {
  this->isActive = isActive;
  Debug::debugMsg(isActive ? "Activated" : "Deactivated");
}

void HttpLogic::updateFieldValue(uint8_t index, int value) {
  if (index>=FIELD_COUNT || index < 0) return;
  currentData[index] = value;
}

bool HttpLogic::updateHttp() {
  bool success = true;

  String query = "/update?api_key=" + apiKey;
  for (uint8_t i=0;i<FIELD_COUNT;i++) {
    addParam(query, i+1, currentData[i]);
  }

  Debug::debugMsg(query.c_str());

  int httpCode = HttpUtils::executeGET("184.106.153.149", 80, query);
  if(httpCode == HTTP_CODE_OK) {
    OUTPUT_SERIAL.println("HTTP OK");
  } else {
    Debug::debugMsg("GET RC:", httpCode);
    success = false;
  }

  return success;
}

bool HttpLogic::checkHttpCmd() {
  bool success = true;

  String query = "/talkbacks/2920/commands/execute?api_key=" + talkbackApiKey_2920;
  int httpCode, contentSize;
  
  String content = HttpUtils::executeGET("184.106.153.149", 80, query, httpCode, contentSize);
  
  if (httpCode == HTTP_CODE_OK) {
    if (contentSize>0 && content.length() > 0) {
      OUTPUT_SERIAL.println("EXEC " + content);
    } else {
      Debug::debugMsg("No new command");
    }
  } else if (httpCode == HTTP_CODE_NOT_MODIFIED) {
    Debug::debugMsg("No new command");
  } else {
    Debug::debugMsg("GET RC:", httpCode);
    success = false;
  }

  return success;
}

void HttpLogic::addParam(String &query, uint8_t index, int value) {
  query+=F("&field");
  query+=index;
  query+=F("=");
  query+=value;
}

