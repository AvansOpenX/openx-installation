#include "PlantLamp.h"

PlantLamp::PlantLamp(byte pin, Adafruit_MCP23X17 &mcp, Preferences &prefs) {
  this->pin = pin;
  this->mcp = mcp;
  this->prefs = prefs;
  init();
}
  
void PlantLamp::init() {
  off();
}

void PlantLamp::on() {
  // Add the time the lamp has been on to activeTime (if the lapm gets 'turned on' when it's already on)
  if (!startTime) {
    activeTime += millis() - startTime;
  }
  // Only turn the lamp on if the maximum number of hours hasn't been reached yet
  if (activeTime / 1000 / 60 / 60 < prefs.getShort("sunHours", 8)) {
    // Set the timestamp for when the lamp lit up
    startTime = millis();
    state = true;
  }
}

void PlantLamp::off() {
  // Add the time the lamp has been on to activeTime
  activeTime += millis() - startTime;
  // Reset the startTime
  startTime = 0;
  state = false;
}


void PlantLamp::resetOnTime() {
  off();
  activeTime = 0;
}