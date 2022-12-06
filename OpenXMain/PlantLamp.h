#ifndef C_PLANT_LAMP
#define C_PLANT_LAMP

#include <Arduino.h>

class PlantLamp {
  private:
    byte pin;
    bool state;

  public:
    PlantLamp(byte pin);
      
    void init();
    void on();
    void off();
}

#endif