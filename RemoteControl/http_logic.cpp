#include "http_logic.h"

#include "password.h"
#include "debug.h"
#include "http_utils.h"

void HttpLogic::init() {
}

void HttpLogic::update() {
  if (lastCheck==0 || millis() - lastCheck > HTTP_CHECK_INTERVAL_MS) {
    checkData();
    lastCheck = millis();
  }
}

bool HttpLogic::postCommand(String cmd) {
  bool success = true;

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

void HttpLogic::checkData() {
  String query = "/talkbacks/6867/commands/929456?api_key=" + talkbackApiKey_6867;
  int httpCode, contentSize;
  String content = HttpUtils::executeGET("184.106.153.149", 80, query, httpCode, contentSize);

  if (httpCode==HTTP_CODE_OK) {
    if (contentSize>0 && content.length()>0) {
      if (!lastData.equals(content)) {
        // new data state
        lastData = content;
        OUTPUT_SERIAL.print("STATE ");
        OUTPUT_SERIAL.println(lastData);
      } else {
        Debug::debugMsg("Not changed");
      }
    } else {
      Debug::debugMsg("Empty result");
    }
  } else {
    Debug::debugMsg("GET RC:", httpCode);
  }
}

