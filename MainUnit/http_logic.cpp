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
  if (lastExecuteUpdate==0 || millis() - lastExecuteUpdate >  updateIntervalMs) {
    if (this->isUpdateActive) {
      if (executeUpdate()) {
        lastExecuteUpdate = millis();
      } else {
        lastExecuteUpdate = millis() - updateIntervalMs + HTTP_UPDATE_RETRY_MS;
        Debug::debugMsg("Update FAILED, Retry in", HTTP_UPDATE_RETRY_MS);
      }
    }
  }

  if (lastExecuteData==0 || millis() - lastExecuteData >  HTTP_DATA_INTERVAL_MS) {
    if (this->isDataActive) {
      if (executeData()) {
        lastExecuteData = millis();
      } else {
        Debug::debugMsg("Data failed");
      }
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

void HttpLogic::setUpdateActive(bool isActive) {
  this->isUpdateActive = isActive;
  Debug::debugMsg(isUpdateActive ? "Update activated" : "Update deactivated");
}

void HttpLogic::setDataActive(bool isActive) {
  this->isDataActive = isActive;
  Debug::debugMsg(isActive ? "Data activated" : "Data deactivated");
}

void HttpLogic::updateFieldValue(uint8_t index, int value) {
  if (index>=FIELD_COUNT || index < 0) return;
  currentData[index] = value;
}

bool HttpLogic::executeUpdate() {
  bool success = true;

  String query = "/update?api_key=" + apiKey;
  
  for (uint8_t i=0;i<FIELD_COUNT;i++) {
    addParam(query, i+1, currentData[i]);
  }

  int httpCode = HttpUtils::executeGET("184.106.153.149", 80, query);
  if(httpCode == HTTP_CODE_OK) {
    OUTPUT_SERIAL.println("HTTP UPDATE OK");
  } else {
    Debug::debugMsg("GET RC:", httpCode);
    success = false;
  }

  return success;
}

bool HttpLogic::executeData() {
  bool success = true;

  String command_string = "";
  command_string.concat(currentData[FIELD_INDEX_WATER]);
  command_string.concat(",");
  command_string.concat(currentData[FIELD_INDEX_HC]);
  command_string.concat(",");
  command_string.concat(currentData[FIELD_INDEX_TANK]);
  command_string.concat(",");
  command_string.concat(currentData[FIELD_INDEX_PUMP_W]);
  command_string.concat(",");
  command_string.concat(currentData[FIELD_INDEX_PUMP_HC]);
  
  String query = "/talkbacks/6867/commands/929456?api_key=" + talkbackApiKey_6867 + "&command_string=" + command_string  + "&position=1";
  int httpCode = HttpUtils::executePUT("184.106.153.149", 80, query, "");
  if(httpCode == HTTP_CODE_OK) {
    OUTPUT_SERIAL.println("HTTP DATA OK");
  } else {
    Debug::debugMsg("GET RC2:", httpCode);
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

