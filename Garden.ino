/*


*/

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <TimeLib.h>
#include "Sensor.h"
#include "Actuator.h"

// Wifi
WiFiClient client;
unsigned long lastConnectionTime = 0;               // track the last connection time
const unsigned long postingInterval = 60L * 1000L;  // post data every 20 seconds

// NTC

// NTP Servers:
IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
//IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
//IPAddress timeServer(17, 172, 224, 47); // time.apple.com NTP server
//IPAddress timeServer(128, 138, 140, 44); // utcnist.colorado.edu NTP server

//const int timeZone = 1;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)

int keyIndex = 0;                     // your network key Index number (needed only for WEP)
unsigned int localPort = 8888;  // local port to listen for UDP packets
// unsigned int localPort = 2390;        // local port to listen for UDP packets

WiFiUDP Udp;                          // A UDP instance to let us send and receive packets over UDP


// Sensor and Actuator Settings

// Temperature Air, Luminosity Air, Temperature Soil, Moisture Soil
TemperatureAirSensor TA = TemperatureAirSensor(A1, 1);
LuminositySensor LA = LuminositySensor(A2, 2);
TemperatureSoilSensor TS = TemperatureSoilSensor(A5, 3);
MoistureSensor MS = MoistureSensor(A4, 4);

Sensor *sensors[] = {
  &TA,
  &LA,
  &TS,
  &MS
};

// Garden 2, 3, 4, 5;
Solenoid G1 = Solenoid(2, 5);
Solenoid G2 = Solenoid(3, 6);
Solenoid G3 = Solenoid(4, 7);
Solenoid G4 = Solenoid(5, 8);

Solenoid *solenoids[] = {
  &G1,
  &G2,
  &G3,
  &G4
};

// Button control
const int buttonPin = 7;
const int buttonCounter = 0;        // Operation mode
int buttonState;
int buttonMode = 0;

// Loop options
const int loopDelay = 10000;

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);

  confirmConnection();

  setTime(0, 50, 00, 18, 7, 2016);
  if (timeStatus() != timeNeedsSync) {
    syncTime();
  }

  // watering plants at
  Alarm.alarmRepeat(7, 0, 0, startWateringS1);
  Alarm.alarmRepeat(7, 10, 0, stopWateringS1);

  Alarm.alarmRepeat(7, 10, 0, startWateringS2);
  Alarm.alarmRepeat(7, 25, 0, stopWateringS2);

  Alarm.alarmRepeat(7, 25, 0, startWateringS3);
  Alarm.alarmRepeat(7, 28, 0, stopWateringS3);

  Alarm.alarmRepeat(7, 30, 0, startWateringS4);
  Alarm.alarmRepeat(7, 35, 0, stopWateringS4);

  pinMode(buttonPin, INPUT);
}

void loop() {

  // confirm Wifi
  confirmConnection();

  // read sensors
  readAllSensors();


  G1.on();


  // check if the pushbutton is pressed
  if (digitalRead(buttonPin)) {
    buttonMode = (buttonMode + 1) % 5;
    // turn LED on
    // TODO
    Serial.print("Button pressed: ");
    Serial.println(buttonMode);
  }

  switch (buttonMode) {
    case 1:
      startWateringS1();
      Alarm.timerOnce(360, stopWateringS1);
      break;
    case 2:
      startWateringS2();
      Alarm.timerOnce(180, stopWateringS2);
      break;
    case 3:
      startWateringS3();
      Alarm.timerOnce(60, stopWateringS3);
      break;
    case 4:
      startWateringS4();
      Alarm.timerOnce(180, stopWateringS4);
      break;
    default:
      Alarm.timerOnce(3, stopWateringS1);
      Alarm.timerOnce(3, stopWateringS2);
      Alarm.timerOnce(3, stopWateringS3);
      Alarm.timerOnce(3, stopWateringS4);
      break;
  }


  Serial.println("--- Watering ---");

  Serial.println(String(hour()) + ":" + String(minute()) + ":" + String(second()));

  // if interval time has passed since the last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    Serial.print("Sending data to web...\n");

    sendToThingSpeak(sensors, solenoids);
  }

  Alarm.delay(loopDelay);        // delay in between reads for stability
}


