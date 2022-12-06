#ifndef C_M_SENSOR
#define C_M_SENSOR

#include <Arduino.h>

class MoistureSensor {
  private:
    byte pin;
    byte index;    

  public:
    MoistureSensor(byte pin, byte index);

    void init();
    int getLevel();
}

#endif