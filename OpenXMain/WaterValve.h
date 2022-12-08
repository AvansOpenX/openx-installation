#ifndef C_WATER_VALVE
#define C_WATER_VALVE

#include <Arduino.h>
#include <Servo.h>

class WaterValve {
  private:
    byte pin;
    byte state;
    Servo *servo;

  public:
    WaterValve(byte pin);

    void init();
    void open(byte percentage);
    void close();
}

#endif