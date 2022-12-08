#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(byte pin, byte index) {
  this->pin = pin;
  this->index = index;
  init();
}

void MoistureSensor::init() {
  // TODO: Retrieve and set limit from storage
  // TODO: Retrieve and set airvalue from storage
  // TODO: Retrieve and set watervalue from storage
  pinMode(pin, INPUT);
}

int MoistureSensor::getLevel() {
  measurement = analogRead(pin);
  // Return percentage based on the calibration values
  return map(measurement, airValue, waterValue, 0, 100);
}

int MoistureSensor::getComparedValue() {
  // Return value should be as close to 0 as possible, negative value means that it needs water
  return getLevel() - limit;
}