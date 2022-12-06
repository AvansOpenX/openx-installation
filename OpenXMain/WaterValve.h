#ifndef C_WATER_VALVE
#define C_WATER_VALVE

#include <Arduino.h>

class WaterValve {
  private:
    byte pin;
    byte state;

  public:
    WaterValve(byte pin) {;

    void init();
    void open(byte percentage);
    void close();
}

#endif