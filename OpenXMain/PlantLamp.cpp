#include "PlantLamp.h"

PlantLamp::PlantLamp(byte pin, Adafruit_MCP23X17 &mcp) {
  this->pin = pin;
  this->mcp = mcp;
  init();
}
  
void PlantLamp::init() {
  
}

void PlantLamp::on() {

}

void PlantLamp::off() {
  
}