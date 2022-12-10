#ifndef C_RESERVOIR
#define C_RESERVOIR

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class Reservoir {
  private:
    Adafruit_MCP23X17 mcp;
    const byte *sensorPins;
    byte valvePin;
    byte pumpPin;
    byte ledPin;

  public:
    Reservoir(const byte sensorPins[], byte valvePin, byte pumpPin, byte ledPin, Adafruit_MCP23X17 &mcp);
    
    void init();
    int getLevel();
    void fill();
    void drain();
};

#endif