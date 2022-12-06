#ifndef C_RESERVOIR
#define C_RESERVOIR

#include <Arduino.h>

class Reservoir {
  private:
    byte pins[];

  public:
    Reservoir(byte pins[]);
    
    void init();
    int getLevel();
    void fill();
    void drain();
}

#endif