#ifndef C_PLANT_LAMP
#define C_PLANT_LAMP

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class PlantLamp {
  private:
    byte pin;
    Adafruit_MCP23X17 mcp;
    unsigned long startTime;
    unsigned long activeTime;

  public:
    PlantLamp(byte pin, Adafruit_MCP23X17 &mcp);
    
    bool state;
    void init();
    void on();
    void off();
    void resetOnTime();
    int getOnTime();
};

#endif