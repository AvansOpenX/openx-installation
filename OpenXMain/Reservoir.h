#ifndef C_RESERVOIR
#define C_RESERVOIR

#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>
#include <Adafruit_MCP23X17.h>

class Reservoir {
  private:
    Adafruit_MCP23X17 mcp;
    Preferences prefs;
    const byte *sensorPins;
    byte valvePin;
    byte pumpPin;
    byte ledPin;
    Servo servo;

  public:
    Reservoir(const byte sensorPins[], byte valvePin, byte pumpPin, byte ledPin, Adafruit_MCP23X17 &mcp, Preferences &prefs);
    
    void init();
    int getLevel();
    void fill();
    unsigned long fillPartially();
    void drain();
    unsigned long drainPartially();
    void close();
    bool isEmpty();
    bool isFull();
};

#endif