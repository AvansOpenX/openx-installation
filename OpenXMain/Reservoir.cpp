#include "Reservoir.h"

Reservoir::Reservoir(const byte sensorPins[], byte valvePin, byte pumpPin, byte ledPin, Adafruit_MCP23X17 &mcp, Preferences &prefs) {
  this->sensorPins = sensorPins;
  this->valvePin = valvePin;
  this->pumpPin = pumpPin;
  this->ledPin = ledPin;
  this->prefs = prefs;
  init();
}

void Reservoir::init() {
  for (byte i = 0; i < sizeof(sensorPins); i++) {
    mcp.pinMode(sensorPins[i], INPUT);
  }
  servo.attach(valvePin);
}

int Reservoir::getLevel() {
  byte level = 0;
  // Count the number of sensors that touch water
  for (byte i = 0; i < sizeof(sensorPins); i++) {
    if (mcp.digitalRead(0)) {
      level++;
    }
  }
  return level;
}

void Reservoir::fill() {
  // TODO: turn waterpump on
}

unsigned long Reservoir::fillPartially() {
  fill();
  // TODO: return timestamp based for when to stop, based on the current level
}

void Reservoir::drain() {
  // Remap the passed percentage to a value between the min and max values
  // TODO: set actual maximum value (now 180)
  servo.write(map(prefs.getShort("rValveFlow"), 0, 100, 0, 180));
}

unsigned long Reservoir::drainPartially() {
  drain();
  // TODO: return timestamp based for when to stop, based on the current level
}

void Reservoir::close() {
  // TODO: turn waterpump off
  servo.write(0);
}

bool Reservoir::isEmpty() {
  return getLevel() == 0;
}

bool Reservoir::isFull() {
  return getLevel() == sizeof(sensorPins);
}