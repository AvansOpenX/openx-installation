#include "WaterValve.h"

WaterValve::WaterValve(byte pin, Preferences &prefs) {
  this->pin = pin;
  this->prefs = prefs;
}

void WaterValve::init(byte index) {
  this->index = index;
  servo.attach(pin);
}

void WaterValve::open() {
  // Remap the passed percentage to a value between the min and max values
  // TODO: set actual maximum value (now 180)
  state = map(prefs.getShort("valveFlow" + index), 0, 100, 0, 180);
  servo.write(state);
}

void WaterValve::close() {
  if (state != 0) {
    servo.write(0);
  }
}