#include "Plant.h"

Plant::Plant(byte index, MoistureSensor moistureSensor, WaterValve waterValve, PlantLamp plantLamp) {
  this->moistureSensor = moistureSensor;
  this->waterValve = waterValve;
  this->lamp = plantLamp;
  init(index);
}

void Plant::init(byte index) {
  moistureSensor->init(index);
  waterValve->init(index);
  lamp->init(index);
}

bool Plant::needsWater() {
  return moistureSensor->getComparedValue() < 0;
}