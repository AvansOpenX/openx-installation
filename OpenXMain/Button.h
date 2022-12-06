#ifndef C_BUTTON
#define C_BUTTON

#include <Arduino.h>

class Button {
  private:
    byte pin;
  
  public: 
    Button(byte pin);

    void init();
    bool isPressed();
}

#endif