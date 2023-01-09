#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(byte pin, Preferences &prefs) {
  this->pin = pin;
  this->prefs = prefs;
  // TODO: calibrate moisture sensors (air and water values)
  this->airValue = 100;
  this->waterValue = 800;
}

void MoistureSensor::init(byte index) {
  this->index = index;
  pinMode(pin, INPUT);
}

int MoistureSensor::getLevel() {
  int measurement = analogRead(pin);
  // Return percentage based on the calibration values
  return map(measurement, airValue, waterValue, 0, 100);
}

int MoistureSensor::getOffset() {
  // Return value should be as close to 0 as possible, negative value means that it needs water
  return getLevel() - prefs.getShort("moistureLimit" + index);
}