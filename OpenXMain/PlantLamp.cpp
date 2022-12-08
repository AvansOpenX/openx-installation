#include "PlantLamp.h"

PlantLamp::PlantLamp(byte pin) {
  this->pin = pin;
  init();
}
  
void PlantLamp::init() {
  pinMode(pin, OUTPUT);
}

void PlantLamp::on() {
  digitalWrite(pin, HIGH);
  state = true;
}

void PlantLamp::off() {
  digitalWrite(pin, LOW);
  state = false;
}