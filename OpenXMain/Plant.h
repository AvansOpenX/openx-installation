#ifndef C_PLANT
#define C_PLANT

#include <Arduino.h>
#include "MoistureSensor.h"
#include "WaterValve.h"
#include "PlantLamp.h"
#include <Preferences.h>

class Plant {
  private:
    MoistureSensor *moistureSensor;
    WaterValve *waterValve;
    Preferences prefs;

  public:
    Plant(byte index, MoistureSensor *moistureSensor, WaterValve *waterValve, PlantLamp *plantLamp, Preferences *prefs);
    
    void init(byte index);
    bool needsWater();
    bool needsLight();
    PlantLamp *plantLamp;
};

#endif