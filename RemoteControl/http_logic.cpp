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

  if (ENABLE_FLASH) {
    if (lastFlashCheck==0 || millis() - lastFlashCheck > HTTP_FLASH_CHECK_INTERVAL_MS) {
      checkData();
      lastFlashCheck = millis();
    }
  }
}

bool HttpLogic::postCommand(String cmd) {
  bool success = true;

  String query = "/talkbacks/2920/commands?api_key=" + talkbackApiKey_2920 + "&command_string=" + cmd;

  int httpCode = HttpUtils::executePOST("184.106.153.149", 80, query, "");
  
  if(httpCode == HTTP_CODE_OK) {
    OUTPUT_SERIAL.println("POST OK");
  } else {
    Debug::debugMsg("GET RC:", httpCode);
    success = false;
  }

  return success;
}

void HttpLogic::checkData(bool forceCheck) {
  String query = "/talkbacks/6867/commands/929456?api_key=" + talkbackApiKey_6867;
  int httpCode, contentSize;
  String content = HttpUtils::executeGET("184.106.153.149", 80, query, httpCode, contentSize);

  if (httpCode==HTTP_CODE_OK) {
    if (forceCheck) {
      lastData = "";
    }

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

void HttpLogic::checkChannel(bool forceCheck) {
  String query = "/channels/41733/feed/last.json?api_key=" + apiKey;
  int httpCode, contentSize;
  String content = HttpUtils::executeGET("184.106.153.149", 80, query, httpCode, contentSize);

  Debug::debugMsg("CHANNEL", content);
  Debug::debugMsg("SIZE", contentSize);

  if (httpCode==HTTP_CODE_OK) {
    if (content.length()>0) {
      if (content.indexOf("{")>=0) {
        int i = content.indexOf(F("\"entry_id\":"));
        if (i>0) {
          int entryId;
          Debug::debugMsg("LAST ", content.substring(i+11));
          
          for (int o=i+11; o<i+20; o++) {
            if (content.substring(o,o+1).equals(",")) {
              Debug::debugMsg("ENTRYSTR", content.substring(i+11,o));
              entryId = content.substring(i+11,o).toInt();
              break;
            }
          }

          if (forceCheck) {
            lastEntryId = 0;
          }

          if (entryId!=lastEntryId) {
            String tmp = "";
            tmp+=getFieldValue(content, 1);
            tmp+=",";
            tmp+=getFieldValue(content, 2);
            tmp+=",";
            tmp+=getFieldValue(content, 3);
            tmp+=",0,0,0";
    
            OUTPUT_SERIAL.print("TEMP ");
            OUTPUT_SERIAL.println(tmp);
            lastEntryId = entryId;
          } else {
            Debug::debugMsg("State not changed");
          }
        }
      } else {
        Debug::debugMsg("No Json response");
      }      
    } else {
      Debug::debugMsg("Empty result");
    }
  } else {
    Debug::debugMsg("GET RC:", httpCode);
  }
}

String HttpLogic::getFieldValue(String data, uint8_t index) {
  String searchStr = "\"field";
  searchStr+=index;
  searchStr+="\":\"";

  int i = data.indexOf(searchStr);
  if (i>=0) {
    i+=searchStr.length();
    
    int o = data.indexOf("\"", i);
    if (o>=0) {
      //Debug::debugMsg("getFieldValue", i, o);
      //Debug::debugMsg("getFieldValue", data.substring(i, o));
      return data.substring(i, o);
    }
  }
  return "";
}

void HttpLogic::checkFlash() {
  String query = "/talkbacks/6877/commands/execute?api_key=" + talkbackApiKey_6877;
  int httpCode, contentSize;
  String content = HttpUtils::executeGET("184.106.153.149", 80, query, httpCode, contentSize);

  if (httpCode==HTTP_CODE_OK) {
    if (contentSize>0 && content.length()>0) {
      if (content.equals("FLASH")) {
        Debug::debugMsg("Flashing");

        t_httpUpdate_return ret = ESPhttpUpdate.update("http://192.168.178.24/RemoteControl.cpp.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Debug::debugMsg("Update failed");
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Debug::debugMsg("No updates");
                break;

            case HTTP_UPDATE_OK:
                Debug::debugMsg("Update ok");
                break;
        }

        
      } else {
        Debug::debugMsg("Unknown flash cmd");
      }
    } else {
      Debug::debugMsg("No flash cmd");
    }
  } else {
    Debug::debugMsg("GET RC:", httpCode);
  }
}

