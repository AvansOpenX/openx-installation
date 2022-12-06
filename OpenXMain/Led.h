#ifndef C_LED
#define C_LED

#include <Arduino.h>

class Led {
  private:
    byte pin;
    byte index;
    int r, g, b, l;
    bool state;

  public:
    Led(byte pin, byte index);
    
    void init();
    void on();
    void off();
    void setColor(int r, int g, int b, int l);
    void toggle();
};

#endif