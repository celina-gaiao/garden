
#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
  public:
    byte pin;
    int tsField;
    float value;

    Sensor(byte _pin, int _tsField) {
      pin = _pin;
      tsField = _tsField;
    };

    virtual int readRaw() {
      return analogRead(pin);
    }

    virtual float read() {
      Serial.print("estou aqui");
      return readRaw();
    }
};

#endif

#ifndef TEMPERATUREAIRSENSOR_H
#define TEMPERATUREAIRSENSOR_H

class TemperatureAirSensor : public Sensor {
  public:
    TemperatureAirSensor(byte _pin, int _tsField) : Sensor(_pin, _tsField) {};

    virtual float read() {
      // converting [0-1024] to [0-3.3]V
      float TemperatureDry = (readRaw() * 3.3) / 1024.0;
      // removing offset
      TemperatureDry = (TemperatureDry - .5) * 100;
      Serial.print("Temperature Air: ");
      Serial.print(TemperatureDry);
      Serial.println("ºC");

      value = TemperatureDry;

      return TemperatureDry;
    }
};

#endif

#ifndef LUMINOSITYSENSOR_H
#define LUMINOSITYSENSOR_H

class LuminositySensor : public Sensor {
  public:
    LuminositySensor(byte _pin, int _tsField) : Sensor(_pin, _tsField) {};

    virtual float read() {

      float Luminosity = (readRaw() * 100)  / 1024;

      Serial.print("Luminosity: ");
      Serial.print(Luminosity);
      Serial.println(" %");

      value = Luminosity;
      return Luminosity;
    }
};

#endif

#ifndef TEMPERATURESOILSENSOR_H
#define TEMPERATURESOILSENSOR_H

class TemperatureSoilSensor : public Sensor {
  public:
    TemperatureSoilSensor(byte _pin, int _tsField) : Sensor(_pin, _tsField) {};

    virtual float read() {

      // converting [0-1024] to [0-3]V, 3 em vez de 3.3 pq 3 é o limite do sensor
      float TemperatureSoil = (readRaw() / 1024.0) * 3;
      TemperatureSoil = TemperatureSoil * 41.67 - 40;
      Serial.print("Temperature Soil: ");
      Serial.print(TemperatureSoil);
      Serial.println("ºC");

      value = TemperatureSoil;
      return TemperatureSoil;
    }
};

#endif

#ifndef MOISTURESENSOR_H
#define MOISTURESENSOR_H

class MoistureSensor : public Sensor {
  public:
    MoistureSensor(byte _pin, int _tsField) : Sensor(_pin, _tsField) {};

    virtual float read() {

      // converting [0-1024] to [0-3]V, 3 em vez de 3.3 pq 3 é o limite do sensor
      float Moisture = (readRaw() / 1024.0) * 3;

      if (Moisture < 1.1)
        Moisture = Moisture * 10 - 1;
      else if (Moisture < 1.3)
        Moisture = Moisture * 25 - 17.5;
      else if (Moisture < 1.82)
        Moisture = Moisture * 48.08 - 47.5;
      else if (Moisture < 2.2)
        Moisture = Moisture * 26.32 - 7.89;

      // from VWC to %
      Moisture = (Moisture * 100) / 3;

      Serial.print("Moisture Soil: ");
      Serial.print(Moisture);
      Serial.println(" %");

      value = Moisture;
      return Moisture;
    }
};

#endif

