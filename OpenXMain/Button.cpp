#include "Button.h"

Button::Button(byte pin, Adafruit_MCP23X17 &mcp) {
  this->pin = pin;
  this->mcp = mcp;
  init();
}

void Button::init() {
  mcp.pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed() {
  state = mcp.digitalRead(0);
  // Compare the current state to the previous state, if the state has changed, return true
  bool rtrVal = state != lastState && state == HIGH;
  lastState = state;
  return rtrVal;
}