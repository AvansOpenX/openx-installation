#ifndef C_PLANT_LAMP
#define C_PLANT_LAMP

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class PlantLamp {
  private:
    byte pin;
    bool state;
    Adafruit_MCP23X17 mcp;

  public:
    PlantLamp(byte pin, Adafruit_MCP23X17 mcp);
      
    void init();
    void on();
    void off();
};

#endif