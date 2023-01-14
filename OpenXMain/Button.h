#ifndef C_BUTTON
#define C_BUTTON

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_NeoPixel.h>
#include "Led.h"

class Button {
  private:
    // Button variables
    byte pin;
    bool state;
    bool lastState;
    Adafruit_MCP23X17 mcp;
    // LED variables
    Adafruit_NeoPixel buttonLeds;
    bool ledState;
    byte ledIndex;
    byte r, g, b;
  
  public: 
    Button(byte pin, Adafruit_MCP23X17 &mcp, Adafruit_NeoPixel &buttonLeds, byte ledIndex);
    // Button methods
    void init();
    bool isPressed();
    // LED methods
    void on();
    void off();
    void setColor(byte r, byte g, byte b);
    void toggle();
};

#endif