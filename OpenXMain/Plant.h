#ifndef C_PLANT
#define C_PLANT

#include <Arduino.h>

class Plant {
  private:

  public:
    Plant();
    
    void init();
    bool needsWater();
}

#endif