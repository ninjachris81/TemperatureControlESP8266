#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

class HttpUtils {
public:
  static bool waitUntilConnected(HTTPClient &httpClient);

  static int executeGET(String host, int port, String query);

  static String executeGET(String host, int port, String query, int &returnCode, int &contentSize);

};


#endif

