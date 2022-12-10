#ifndef C_WATER_VALVE
#define C_WATER_VALVE

#include <Arduino.h>
#include <ESP32Servo.h>

class WaterValve {
  private:
    byte pin;
    byte index;
    byte state;
    Servo servo;

  public:
    WaterValve(byte pin);

    void init(byte index);
    void open(byte percentage);
    void close();
};

#endif