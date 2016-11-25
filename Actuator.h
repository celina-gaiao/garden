
#ifndef ACTUATOR_H
#define ACTUATOR_H

class Actuator {
  public:
    byte pin;
    int tsField;
    int value;

    Actuator(byte _pin) {
      create(_pin, 0);
    };

    Actuator(byte _pin, int _tsField) {
      create(_pin, _tsField);
    };
   
  private:
    virtual int create(byte _pin, int _tsField) {
      pin = _pin;
      tsField = _tsField;
      return pin;
    }
};

#endif

#ifndef SOLENOID_H
#define SOLENOID_H

class Solenoid : public Actuator {
  public:
    Solenoid(byte _pin) : Actuator(_pin) {};
    Solenoid(byte _pin, int _tsField) : Actuator(_pin, _tsField) {};

    int wateringTime;

    void on() {
      pinMode(pin, OUTPUT);
      value = 1;
      
      Serial.println("Opening " + String(pin) + " now");
      digitalWrite(pin, HIGH);
    }
    
    void off() {
      pinMode(pin, OUTPUT);
      value = 0;
      
      Serial.println("Closing " + String(pin) + " now");
      digitalWrite(pin, LOW);
      
    }

};

#endif
