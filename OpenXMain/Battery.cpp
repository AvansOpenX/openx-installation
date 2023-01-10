#include "Battery.h"

Battery::Battery() {
  init();
}

void Battery::init() {
  percentage = 50;
}

int Battery::getLevel() {
  return percentage;
}

void Battery::fill() {
  percentage += (100 - percentage) / 3;
}

void Battery::drain() {
  if (percentage) {
    percentage--;
  }
}