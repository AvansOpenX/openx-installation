#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(byte pin, byte index) {
  this->pin = pin;
  this->index = index;
  init();
}

void MoistureSensor::init() {

}

int MoistureSensor::getLevel() {
  
}