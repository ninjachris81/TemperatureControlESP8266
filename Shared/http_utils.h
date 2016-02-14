#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define HTTP_CODE_OK 200
#define HTTP_CODE_NOT_MODIFIED 304

class HttpUtils {
public:
  static int executeGET(String host, int port, String query);

  static String executeGET(String host, int port, String query, int &returnCode, int &contentSize);

  static int executePOST(String host, int port, String query, String postData);

  static String executePOST(String host, int port, String query, String postData, int &returnCode, int &contentSize);

  static int executePUT(String host, int port, String query, String putData);

  static int executeDELETE(String host, int port, String query);

  };


#endif

