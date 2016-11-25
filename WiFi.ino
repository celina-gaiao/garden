
#include "Credentials.h"

void confirmConnection() {

  int status = WiFi.status();

  if (status == WL_CONNECTED) {
    return;
  }

  if (status == WL_NO_SHIELD) {
    // uploading no Mega
    return;
  }

  int attempts = 0;

  Serial.print("Connecting to wifi..");
  // attempt to connect to Wifi network
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 Wi-Fi network
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    Serial.print(".");
    // wait 1 seconds for connection
    delay(1000);

    if (attempts++ > 2) {
      Serial.println("ERROR");
      return;
    }
  }

  Serial.println("SUCCESS");
}

