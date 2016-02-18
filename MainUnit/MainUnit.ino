#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "globals.h"
#include "ntp_logic.h"
#include "http_logic.h"
#include "debug.h"
#include "password.h"

ESP8266WiFiMulti wiFiMulti;

uint8_t timeout = 0;

NtpLogic ntpLogic;
HttpLogic httpLogic;

void inputError(const char* msg, String param1) {
  OUTPUT_SERIAL.print(">");
  OUTPUT_SERIAL.print(msg);
  OUTPUT_SERIAL.print(" ");
  OUTPUT_SERIAL.println(param1);
}

bool checkInput() {
  if (OUTPUT_SERIAL.available()>0) {
    String tmp = OUTPUT_SERIAL.readStringUntil('\n');
    if (tmp.length()>0) {
      Debug::debugMsg("INPUT", tmp);
      
      if (tmp.startsWith("HTTP ")) {
        tmp = tmp.substring(5);
        if (tmp.startsWith("FIELD ")) {
          tmp = tmp.substring(6);
          int i = tmp.indexOf(' ');
          if (i>0) {
            String fieldIndexStr = tmp.substring(0,i);
            String valueStr = tmp.substring(i+1);
            fieldIndexStr.trim();
            valueStr.trim();
            
            Debug::debugMsg("FI", fieldIndexStr);
            Debug::debugMsg("VAL", valueStr);

            httpLogic.updateFieldValue(fieldIndexStr.toInt(), valueStr.toInt());
          } else {
            // syntax error
            inputError("Invalid HTTP FIELD Cmd", tmp);
          }
        } else if (tmp.startsWith("UPDATE ")) {
          tmp = tmp.substring(7);
          if (tmp.startsWith("ON")) {
            httpLogic.setUpdateActive(true);
          } else if (tmp.startsWith("OFF")) {
            httpLogic.setUpdateActive(false);
          } else if (tmp.startsWith("SEND")) {
            httpLogic.executeUpdate();
          } else {
            inputError("Invalid HTTP UPDATE Cmd", tmp);
          }
        } else if (tmp.startsWith("DATA ")) {
          tmp = tmp.substring(5);
          if (tmp.startsWith("SEND")) {
            httpLogic.sendData();
          } else {
            inputError("Invalid HTTP DATA Cmd", tmp);
          }
        } else {
          inputError("Invalid HTTP Cmd", tmp);
        }
      } else if (tmp.startsWith("NTP ")) {
        tmp = tmp.substring(4);
        if (tmp.startsWith("INTERVAL ")) {
          tmp = tmp.substring(9);
          tmp.trim();

          int tmpInterval = tmp.toInt();
          if (tmpInterval>=1000) {
            Debug::debugMsg("NTP INTV", tmpInterval);
            ntpLogic.setInterval(tmpInterval);
          } else {
            inputError("Invalid NTP Cmd", tmp);
          }
        } else if (tmp.startsWith("SYNC")) {
          ntpLogic.updateTime();
        } else {
          inputError("Invalid NTP Cmd", tmp);
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
  
  ntpLogic.init();
}

void loop() {
  if (checkInput()) {
    // parse input
  }

  timeout = 0;
  while (WiFi.status() != WL_CONNECTED) {
    wiFiMulti.run(); 
    delay(500);
    timeout++;
    if (timeout>10) {
      Debug::debugMsg("WIFI ERR");
      break;
    }
  }

  httpLogic.update();

  ntpLogic.update();

  delay(100);
}
