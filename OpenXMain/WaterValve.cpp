#include "WaterValve.h"

WaterValve::WaterValve(byte pin) {
  this->pin = pin;
}

void WaterValve::init(byte index) {
  this->index = index;
  servo.attach(pin);
}

void WaterValve::open(byte percentage) {
  // Remap the passed percentage to a value between the min and max values
  state = map(percentage, 0, 100, 0, 180);
  servo.write(state);
}

void WaterValve::close() {
  if (state != 0) {
    servo.write(0);
  }
}