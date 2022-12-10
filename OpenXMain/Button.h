#ifndef C_BUTTON
#define C_BUTTON

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class Button {
  private:
    byte pin;
    Adafruit_MCP23X17 mcp;
  
  public: 
    Button(byte pin, Adafruit_MCP23X17 &mcp);

    void init();
    bool isPressed();
};

#endif