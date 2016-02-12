#include "http_utils.h"

bool HttpUtils::waitUntilConnected(HTTPClient &httpClient) {
  uint8_t timeout = 0;
  while(!httpClient.connected()) {
    delay(250);
    timeout++;
    if (timeout>20) {
      return false;
    }
  }
  return true;
}

int HttpUtils::executeGET(String host, int port, String query) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");
  
  httpClient.begin(host, port, query); //HTTP
  HttpUtils::waitUntilConnected(httpClient);
  int returnCode = httpClient.GET();
  httpClient.end();
  return returnCode;
}

String HttpUtils::executeGET(String host, int port, String query, int &returnCode, int &contentSize) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");

  httpClient.begin(host, port, query); //HTTP
  HttpUtils::waitUntilConnected(httpClient);
  returnCode = httpClient.GET();
  contentSize = httpClient.getSize();
  String content = httpClient.getString();
  httpClient.end();
  return content;
}


