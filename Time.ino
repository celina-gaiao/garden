
void syncTime() {
  Serial.println("Starting UDP...");
  Udp.begin(localPort);
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
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
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}



/* TODO it was here before

  unsigned long inline ntpUnixTime (UDP &udp)
  {
  static int udpInited = udp.begin(123); // open socket on arbitrary port

  const char timeServer[] = "pool.ntp.org";  // NTP server

  // Only the first four bytes of an outgoing NTP packet need to be set
  // appropriately, the rest can be whatever.
  const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

  // Fail if WiFiUdp.begin() could not init a socket
  if (! udpInited)
    return 0;

  // Clear received data from possible stray received packets
  udp.flush();

  // Send an NTP request
  if (! (udp.beginPacket(timeServer, 123) // 123 is the NTP port
   && udp.write((byte *)&ntpFirstFourBytes, 48) == 48
   && udp.endPacket())) {
    Serial.println("Unable to contact NTP server");
    return 0;       // sending request failed
   }


  // Wait for response; check every pollIntv ms up to maxPoll times
  const int pollIntv = 150;   // poll every this many ms
  const byte maxPoll = 15;    // poll up to this many times
  int pktLen;       // received packet length
  for (byte i=0; i<maxPoll; i++) {
    if ((pktLen = udp.parsePacket()) == 48)
      break;
    delay(pollIntv);
  }
  if (pktLen != 48) {
    Serial.println("FUCK");
    return 0;       // no correct packet received
  }

  // Read and discard the first useless bytes
  // Set useless to 32 for speed; set to 40 for accuracy.
  const byte useless = 40;
  for (byte i = 0; i < useless; ++i)
    udp.read();

  // Read the integer part of sending time
  unsigned long time = udp.read();  // NTP time
  for (byte i = 1; i < 4; i++)
    time = time << 8 | udp.read();

  // Round to the nearest second if we want accuracy
  // The fractionary part is the next byte divided by 256: if it is
  // greater than 500ms we round to the next second; we also account
  // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
  // additionally, we account for how much we delayed reading the packet
  // since its arrival, which we assume on average to be pollIntv/2.
  time += (udp.read() > 115 - pollIntv/8);

  // Discard the rest of the packet
  udp.flush();

  return time - 2208988800ul;   // convert NTP time to Unix time
  }


  unsigned long syncTime(UDP &udp) {
  if ((millis() - lastUDPUpdate) > udpInterval) {
    Serial.println("Asking time...");

    Serial.println(ntpUnixTime(udp));
    lastUDPUpdate = millis();
  }
  }
*/

//
// NTP request
//
// send an NTP request to the time server at the given address
//unsigned long sendNTPpacket(IPAddress & address) {
//  //Serial.println("1");
//  // set all bytes in the buffer to 0
//  memset(packetBuffer, 0, NTP_PACKET_SIZE);
//  // Initialize values needed to form NTP request
//  // (see URL above for details on the packets)
//  //Serial.println("2");
//  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
//  packetBuffer[1] = 0;     // Stratum, or type of clock
//  packetBuffer[2] = 6;     // Polling Interval
//  packetBuffer[3] = 0xEC;  // Peer Clock Precision
//  // 8 bytes of zero for Root Delay & Root Dispersion
//  packetBuffer[12]  = 49;
//  packetBuffer[13]  = 0x4E;
//  packetBuffer[14]  = 49;
//  packetBuffer[15]  = 52;
//
//  //Serial.println("3");
//
//  // all NTP fields have been given values, now
//  // you can send a packet requesting a timestamp:
//  Udp.beginPacket(address, 123); //NTP requests are to port 123
//  //Serial.println("4");
//  Udp.write(packetBuffer, NTP_PACKET_SIZE);
//  //Serial.println("5");
//  Udp.endPacket();
//  //Serial.println("6");
//}
//
//void readTime() {
//
//if (!Udp.parsePacket()) {
//  Serial.println("not working...");
//  return;
//}
//
//  // We've received a packet, read the data from it
//  Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
//  for (int i = 15; i < NTP_PACKET_SIZE; i++) {
//    Serial.print(packetBuffer[i]);
//  }
//  Serial.println();
//  //the timestamp starts at byte 40 of the received packet and is four bytes,
//  // or two words, long. First, extract the two words:
//  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
//  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
//
//  Serial.println(highWord);
//  Serial.println(lowWord);
//
//  // combine the four bytes (two words) into a long integer
//  // this is NTP time (seconds since Jan 1 1900):
//  unsigned long secsSince1900 = highWord << 16 | lowWord;
//  Serial.print("Seconds since Jan 1 1900 = ");
//  Serial.println(secsSince1900);
//
//  // now convert NTP time into everyday time:
//  Serial.print("Unix time = ");
//  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
//  const unsigned long seventyYears = 2208988800UL;
//  // subtract seventy years:
//  unsigned long epoch = secsSince1900 - seventyYears;
//  // print Unix time:
//  Serial.println(epoch);
//
//  // print the hour, minute and second:
//  // UTC is the time at Greenwich Meridian (GMT)
//  time.hour = (epoch  % 86400L) / 3600; // hour (86400 equals secs per day)
//  time.minute = (epoch  % 3600) / 60; // print the minute (3600 equals secs per minute)
//  time.second = epoch % 60; // print the second
//
//  Serial.print(time.hour); Serial.print(':');
//  Serial.print(time.minute); Serial.print(':');
//  Serial.println(time.second);
//
//}





void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}



void printDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
