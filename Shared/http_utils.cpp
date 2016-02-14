#include "http_utils.h"
#include "debug.h"

int HttpUtils::executeGET(String host, int port, String query) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");

  Debug::debugMsg("GET", query);
  
  httpClient.begin(host, port, query); //HTTP
  int returnCode = httpClient.GET();
  httpClient.end();
  return returnCode;
}

String HttpUtils::executeGET(String host, int port, String query, int &returnCode, int &contentSize) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");

  Debug::debugMsg("GET", query);

  httpClient.begin(host, port, query); //HTTP
  returnCode = httpClient.GET();
  contentSize = httpClient.getSize();
  String content = httpClient.getString();
  httpClient.end();
  return content;
}

int HttpUtils::executePOST(String host, int port, String query, String postData) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");
  
  Debug::debugMsg("POST", query);

  httpClient.begin(host, port, query); //HTTP
  int returnCode = httpClient.POST(postData);
  httpClient.end();
  return returnCode;
}

String HttpUtils::executePOST(String host, int port, String query, String postData, int &returnCode, int &contentSize) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");

  Debug::debugMsg("POST", query);

  httpClient.begin(host, port, query); //HTTP
  returnCode = httpClient.POST(postData);
  contentSize = httpClient.getSize();
  String content = httpClient.getString();
  httpClient.end();
  return content;
}

int HttpUtils::executePUT(String host, int port, String query, String putData) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");
  
  Debug::debugMsg("PUT", query);

  httpClient.begin(host, port, query); //HTTP
  int returnCode = httpClient.sendRequest("PUT", (uint8_t *) putData.c_str(), putData.length());
  httpClient.end();
  return returnCode;
}

int HttpUtils::executeDELETE(String host, int port, String query) {
  HTTPClient httpClient;
  httpClient.setUserAgent("TempControl");
  
  Debug::debugMsg("DELETE", query);

  String noData = "";
  httpClient.begin(host, port, query); //HTTP
  int returnCode = httpClient.sendRequest("DELETE", (uint8_t *) noData.c_str(), noData.length());
  httpClient.end();
  return returnCode;
}


