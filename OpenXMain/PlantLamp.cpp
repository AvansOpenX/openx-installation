#include "PlantLamp.h"

PlantLamp::PlantLamp(byte pin, Adafruit_MCP23X17 &mcp) {
  this->pin = pin;
  this->mcp = mcp;
  init();
}
  
void PlantLamp::init() {

}

void PlantLamp::on() {
  if (!state) {
    // Set the timestamp for when the lamp lit up
    startTime = millis();
    state = true;
  }
}

void PlantLamp::off() {
  if (state) {
    // Add the time the lamp has been on to activeTime
    activeTime += millis() - startTime;
    // Reset the startTime
    startTime = 0;
    state = false;
  }
}

void PlantLamp::resetOnTime() {
  off();
  activeTime = 0;
}

int PlantLamp::getOnTime() {
  return (activeTime + startTime ? millis() - startTime : 0) / 1000 / 60 / 60;
}
