#ifndef C_WATER_VALVE
#define C_WATER_VALVE

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>

class WaterValve {
  private:
    byte pin;
    byte index;
    byte state;
    Servo servo;
    Preferences prefs;

  public:
    WaterValve(byte pin, Preferences &prefs);

    void init(byte index);
    void open();
    void close();
};

#endif