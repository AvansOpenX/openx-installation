#include "WaterValve.h"

WaterValve::WaterValve(byte pin) {
  this->pin = pin;
  init();
}

void WaterValve::init() {
  servo.attach(pin);
}

void WaterValve::open(byte percentage) {
  // Remap the passed percentage to a value between the min and max values
  state = map(percentage, 0, 0, 180);
  servo.write(state);
}

void WaterValve::close() {
  if (state != 0) {
    servo.write(0);
  }
}