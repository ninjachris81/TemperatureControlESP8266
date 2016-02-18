#include "http_logic.h"
#include "debug.h"
#include "http_utils.h"

HttpLogic::HttpLogic() {
  server = new WiFiServer(8080);
}

HttpLogic::~HttpLogic() {
  delete server;
}

void HttpLogic::init() {
  Debug::debugMsg("INIT HTTP");

  for (uint8_t i=0;i<FIELD_COUNT;i++) {
    currentData[i] = 0;
  }

  server->begin();
}

void HttpLogic::update() {
  if (!client) {
    client = server->available();
  } else {
    if (client.status()==CLOSED) {
      Debug::debugMsg("Stopping client");
      client.stop();
      delay(50);
    }
  }
  
  if (client && client.connected()) {
    //Debug::debugMsg("Client connected");
    
    if(client.available()) {
      String content = client.readStringUntil('\n');

      if (content.length()>0) {
        OUTPUT_SERIAL.println("EXEC " + content);
      } else {
        Debug::debugMsg("No content received");
      }
    }
  } else {
    //Debug::debugMsg("Client not connected");
  }
  
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
}

void HttpLogic::setUpdateActive(bool isActive) {
  this->isUpdateActive = isActive;
  Debug::debugMsg(isUpdateActive ? "Update activated" : "Update deactivated");
}

void HttpLogic::updateFieldValue(uint8_t index, int value) {
  if (index>=FIELD_COUNT || index < 0) return;
  currentData[index] = value;

  sendData();
}

bool HttpLogic::executeUpdate() {
  bool success = true;

  String query = "/update?api_key=" + apiKey;
  
  for (uint8_t i=0;i<FIELD_UPDATE_COUNT;i++) {
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

void HttpLogic::sendData() {
  if (client && client.connected()) {
    String command_string = "";
    command_string.concat(currentData[FIELD_INDEX_WATER]);
    command_string.concat(",");
    command_string.concat(currentData[FIELD_INDEX_HC]);
    command_string.concat(",");
    command_string.concat(currentData[FIELD_INDEX_TANK]);
    command_string.concat(",");
    command_string.concat(currentData[FIELD_INDEX_PUMP_WATER_ON]);
    command_string.concat(",");
    command_string.concat(currentData[FIELD_INDEX_PUMP_HC_ON]);
    command_string.concat(",");
    command_string.concat(currentData[FIELD_INDEX_TS]);
    
    OUTPUT_SERIAL.println("DATA SENT");

    client.println(command_string);
    client.flush();
  } else {
    Debug::debugMsg("No client connected");
  }
}

void HttpLogic::addParam(String &query, uint8_t index, int value) {
  query+=F("&field");
  query+=index;
  query+=F("=");
  query+=value;
}

