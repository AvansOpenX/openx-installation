#include "Button.h"

Button::Button(byte pin, Adafruit_MCP23X17 mcp) {
  this->pin = pin;
  this->mcp = mcp;
  init();
}

void Button::init() {
  mcp.pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed() {
  return mcp1.digitalRead(0);
}