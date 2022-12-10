#include "Reservoir.h"

Reservoir::Reservoir(const byte sensorPins[], byte valvePin, byte pumpPin, byte ledPin, Adafruit_MCP23X17 &mcp) {
  this->sensorPins = sensorPins;
  this->valvePin = valvePin;
  this->pumpPin = pumpPin;
  this->ledPin = ledPin;
  init();
}

void Reservoir::init() {
  for (byte i = 0; i < sizeof(sensorPins); i++) {
    mcp.pinMode(sensorPins[i], INPUT);
  }
}

int Reservoir::getLevel() {
  byte level = 0;
  for (byte i = 0; i < sizeof(sensorPins); i++) {
    if (mcp.digitalRead(0)) {
      level++;
    }
  }
  return level;
}

void Reservoir::fill() {

}

void Reservoir::drain() {

}