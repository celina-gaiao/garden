
#include "Credentials.h"

// ThingSpeak
char server[] = "api.thingspeak.com";
String writeAPIKey = THINGSPEAK_API_KEY;

void sendToThingSpeak(Sensor* sensors[], Solenoid* solenoids[]) {

  // create data string to send to ThingSpeak
  String data;
  for (int i = 0; i < 4; i++) {
    data += "field" + String(sensors[i]->tsField) + "=" + String(sensors[i]->read(), DEC) + "&";
  }

  for (int i = 0; i < 4; i++) {
    data += "field" + String(solenoids[i]->tsField) + "=" + String(solenoids[i]->value, DEC) + "&";
  }

  Serial.println(data);

  // close any connection before sending a new request
  client.stop();

  // POST data to ThingSpeak
  if (client.connect(server, 80)) {
    client.println("POST /update HTTP/1.1");
    client.println("Host: " + String(server));
    //client.println("Connection: close");
    //    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data);

    // note the last connection time
    lastConnectionTime = millis();
  }
}



