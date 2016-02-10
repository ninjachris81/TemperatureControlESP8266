#ifndef NTP_LOGIC_H
#define NTP_LOGIC_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "globals.h"

#ifdef IS_DEBUG
  #define NTP_INTERVAL_MS 60000
#else
  #define NTP_INTERVAL_MS 3600000
#endif

#define NTP_PACKET_SIZE 48

class NtpLogic {
public:
  void init();

  void update();

  void setInterval(unsigned int intervalMs);

  bool updateTime();

private:
  long lastNTPUpdate = 0;

  unsigned long intervalMs = NTP_INTERVAL_MS;

  unsigned long sendNTPpacket(IPAddress& address, byte *packetBuffer, WiFiUDP *udp);
  
};


#endif
