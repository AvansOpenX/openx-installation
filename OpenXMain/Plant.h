#ifndef C_PLANT
#define C_PLANT

#include <Arduino.h>
#include "MoistureSensor.h"
#include "WaterValve.h"
#include "PlantLamp.h"

class Plant {
  private:
    MoistureSensor *moistureSensor;
    WaterValve *waterValve;
    PlantLamp *plantLamp;

  public:
    Plant(byte index, MoistureSensor *moistureSensor, WaterValve *waterValve, PlantLamp *plantLamp);
    
    void init(byte index);
    bool needsWater();
};

#endif