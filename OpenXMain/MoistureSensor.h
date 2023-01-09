#ifndef C_M_SENSOR
#define C_M_SENSOR

#include <Arduino.h>
#include <Preferences.h>

class MoistureSensor {
  private:
    byte pin;
    byte index;
    int airValue;
    int waterValue;
    Preferences prefs;

  public:
    MoistureSensor(byte pin, Preferences &prefs);

    void init(byte index);
    int getLevel();
    int getOffset();
};

#endif