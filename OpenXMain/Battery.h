#ifndef C_BATTERY
#define C_BATTERY

#include <Arduino.h>

class Battery {
  private:
    byte pin;
    byte percentage;

  public:
    Battery(byte pin);

    void init();
    int getLevel();
    void fill();
    void drain();
};

#endif