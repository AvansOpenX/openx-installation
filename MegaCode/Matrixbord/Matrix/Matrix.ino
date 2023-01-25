#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h> //for the fonts
#include <Fonts/FreeSerif9pt7b.h>

//matrix bord
#define CLK 11   
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define RXD1 19

//scores
int highscore;
int scoreP1 = 99;
int scoreP2 = 123;


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)


void setup() {

  Serial.begin(9600);


  matrix.begin();
  
}


void loop(){

  //TO DO als de esp niet anders aangeeft doe dit

  //TO DO tussen de spellen door highscore laten zien

  //TO DO tussen spellen door studenten motiveren om te spelen

  endGame();

  //delay(4000);

  //highscore = analogRead(Score);
}


void showHighscore(){  
  matrix.setCursor(3, 6);    // (x, y)
  matrix.print("Highscore: ");
  //next line 
  matrix.setCursor(23, 18);
  //matrix.print(highscore);
  //delay(4000);
  matrix.clear();

}

void endGame(){

  showWinner();

  delay(4000);

  showScoreP1();
  delay(2000);
  showScoreP2();
}



