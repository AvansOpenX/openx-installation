#include "Plant.h"

Plant::Plant(byte index, MoistureSensor *moistureSensor, WaterValve *waterValve, PlantLamp *plantLam, Preferences &prefs) {
  this->moistureSensor = moistureSensor;
  this->waterValve = waterValve;
  this->plantLamp = plantLamp;
  this->prefs = prefs;
  init(index);
}

void Plant::init(byte index) {
  moistureSensor->init(index);
  waterValve->init(index);
  plantLamp->init();
}

bool Plant::needsWater() {
  return moistureSensor->getOffset() < 0;
}

bool Plant::needsLight() {
  return plantLamp->getOnTime() < prefs.getShort("sunHours", 8);
}
