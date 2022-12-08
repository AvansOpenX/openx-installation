#ifndef C_M_SENSOR
#define C_M_SENSOR

#include <Arduino.h>

class MoistureSensor {
  private:
    byte pin;
    byte index;
    int limit;
    int airValue;
    int waterValue;

  public:
    MoistureSensor(byte index, byte pin);

    void init();
    int getLevel();
    int getComparedValue();
}

#endif