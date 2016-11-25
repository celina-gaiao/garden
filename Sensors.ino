
void readAllSensors() {

  Serial.write("\n");
  Serial.println("--- Sensors ---");
  
  // read the input on analog pins on the sensors array
  for (int i = 0; i < sizeof(sensors)/sizeof(int); i++) {
    sensors[i]->read();
  }
 
  Serial.write("\n");
}
