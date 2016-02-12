#include "http_logic.h"

#include "password.h"
#include "debug.h"
#include "http_utils.h"

void HttpLogic::init() {
  
}

void HttpLogic::update() {
}

bool HttpLogic::postCommand(String cmd) {
  bool success = true;
  HTTPClient http;
  http.setUserAgent("TempControl");

  String query = "/talkbacks/2920/commands?api_key=?api_key=" + talkbackApiKey_2920 + "&command_string=" + cmd;
  Debug::debugMsg(query.c_str());

  int httpCode = HttpUtils::executePOST("184.106.153.149", 80, query, "");
  
  if(httpCode == HTTP_CODE_OK) {
    OUTPUT_SERIAL.println("POST OK");
  } else {
    Debug::debugMsg("GET RC:", httpCode);
    success = false;
  }

  return success;
}

void HttpLogic::syncData() {
  
}

