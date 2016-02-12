#include "ntp_logic.h"
#include "debug.h"

void NtpLogic::init() {
  Debug::debugMsg("INIT NTP");
}

void NtpLogic::setInterval(unsigned int intervalMs) {
  this->intervalMs = intervalMs;
}

void NtpLogic::update() {
  if (lastNTPUpdate==0 || millis() - lastNTPUpdate > intervalMs) {
    if (updateTime()) {
      lastNTPUpdate = millis();
    } else {
      lastNTPUpdate = millis() - (intervalMs / 10);   // retry
      Debug::debugMsg("Re-try after:", millis(), lastNTPUpdate);
    }
  }
}

bool NtpLogic::updateTime() {
  unsigned int localPort = 2390;      // local port to listen for UDP packets
  /* Don't hardwire the IP address or we won't get the benefits of the pool.
   *  Lookup the IP address for the host name instead */
  //IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
  IPAddress timeServerIP; // time.nist.gov NTP server address
  const char* ntpServerName = "time.nist.gov";
  byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
  WiFiUDP udp;
  bool success = false;

  // We start by connecting to a WiFi network
  Debug::debugMsg("CONN0");
  
  udp.begin(localPort);

  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP, packetBuffer, &udp); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    Debug::debugMsg("UDP ERR");
    success = false;
  } else {
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    /*
    OUTPUT_SERIAL.print("Seconds since Jan 1 1900 = " );
    OUTPUT_SERIAL.println(secsSince1900);
    */

    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    OUTPUT_SERIAL.print("UT=");
    OUTPUT_SERIAL.println(epoch);


    // print the hour, minute and second:
    success = true;
  }

  Debug::debugMsg("DIS0");
  udp.stop();

  if (success) {
    Debug::debugMsg("SLEEP");
  }

  return success;
}

// send an NTP request to the time server at the given address
unsigned long NtpLogic::sendNTPpacket(IPAddress& address, byte *packetBuffer, WiFiUDP *udp) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp->beginPacket(address, 123); //NTP requests are to port 123
  udp->write(packetBuffer, NTP_PACKET_SIZE);
  udp->endPacket();
}
