#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "globals.h"
#include "http_logic.h"
#include "debug.h"
#include "password.h"

ESP8266WiFiMulti wiFiMulti;

HttpLogic httpLogic;

uint8_t timeout = 0;

void inputError(const char* msg, String param1) {
  OUTPUT_SERIAL.print(">");
  OUTPUT_SERIAL.print(msg);
  OUTPUT_SERIAL.print(" ");
  OUTPUT_SERIAL.println(param1);
}

bool checkInput() {
  if (OUTPUT_SERIAL.available() > 0) {
    String tmp = OUTPUT_SERIAL.readStringUntil('\n');
    if (tmp.length() > 0) {
      Debug::debugMsg("INPUT", tmp);

      if (tmp.startsWith("HTTP ")) {
        tmp = tmp.substring(5);
        if (tmp.startsWith("CMD ")) {
          tmp = tmp.substring(6);
          tmp.trim();

          Debug::debugMsg("CMD", tmp);
          httpLogic.postCommand(tmp);
        } else if (tmp.startsWith("CHECK")) {
          httpLogic.checkData();
        }
      } else if (tmp.startsWith("DEBUG ")) {
        tmp = tmp.substring(6);

        if (tmp.startsWith("OFF")) {
          Debug::debugMsg("Disabling debug");
          Debug::isDebug = false;
        } else if (tmp.startsWith("ON")) {
          Debug::isDebug = true;
          Debug::debugMsg("Debug enabled");
        } else {
          inputError("Invalid DEBUG Cmd", tmp);
        }
      } else {
        inputError("Invalid Cmd", tmp);
      }
    } else {
      Debug::debugMsg("Input end not found");
    }
  }
}

void setup() {
  OUTPUT_SERIAL.begin(115200);

  wiFiMulti.addAP(ssid.c_str(), pass.c_str());

  httpLogic.init();
}

void loop() {
  timeout = 0;

  while (WiFi.status() != WL_CONNECTED) {
    wiFiMulti.run();
    delay(500);
    timeout++;
    if (timeout > 10) {
      Debug::debugMsg("WIFI ERR");
      break;
    }
  }

  httpLogic.update();

  delay(500);
}
