#ifndef C_SCREEN
#define C_SCREEN

#include <Arduino.h>

class Screen {
  private:
    byte pin;

  public:
    Screen(byte pin);

    void showHighscore(int highscore);
    void showScore(int score);
    void showScore(int score1, int score2);
    void idle();
}

#endif