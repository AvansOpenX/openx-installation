#include "Button.h"

// Button methods
Button::Button(byte pin, Adafruit_MCP23X17 &mcp, Adafruit_NeoPixel &buttonLeds, byte ledIndex) {
  this->pin = pin;
  this->mcp = mcp;
  this->buttonLeds = buttonLeds;
  init();
}

void Button::init() {
  mcp.pinMode(pin, INPUT_PULLUP);
  led->init();
}

bool Button::isPressed() {
  state = mcp.digitalRead(pin);
  // Compare the current state to the previous state, if the state has changed, return true
  bool rtrVal = state != lastState && state == HIGH;
  lastState = state;
  return rtrVal;
}

// LED methods
void Button::on() {
  ledState = true;
  buttonLeds.setPixelColor(ledIndex, pixels.Color(r, g, b));
  buttonLeds.show();
}

void Button::off() {
  ledState = false;
  buttonLeds.setPixelColor(ledIndex, pixels.Color(0, 0, 0));
  buttonLeds.show();
}

void Button::setColor(byte r, byte g, byte b) {
  this->r = r;
  this->g = g;
  this->b = b;
  // Update the led if the led is already on
  if (ledState) on();
}

void Button::toggle() {
  ledState = !ledState;
  buttonLeds.setPixelColor(ledIndex, ledState ? pixels.Color(r, g, b) : pixels.Color(0, 0, 0));
  buttonLeds.show();
}
