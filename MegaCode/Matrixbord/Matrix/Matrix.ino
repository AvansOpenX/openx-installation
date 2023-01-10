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

//seriele communicatie
#define Score A5

//scores
int highscore;
int scoreP1;
int scoreP2;


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)


void setup() {

  Serial.begin(9600);

  matrix.begin();
  pinMode(INPUT, Score);
  
}


void loop(){

  showScoreP2();

  delay(4000);

  //highscore = analogRead(Score);
}


void showHighscore(){  
  matrix.setCursor(3, 6);    // (x, y)
  matrix.print("Highscore: ");
  //next line 
  matrix.setCursor(23, 18);
  //matrix.print(highscore);
  delay(4000);
  matrix.clear();

}

//show score if one player played
void showScoreP1(){

}

//show score if two players played
void showScoreP2(){

  int scoreP1 = 199;
  int scoreP2 = 256;

  matrix.setCursor(2, 6);    // (x, y)
  matrix.print("Player1:");
  matrix.print(scoreP1);

  //next line 
  matrix.setCursor(2, 18);
  matrix.print("Player2:");
  matrix.print(scoreP2);

  delay(10000);
  matrix.clear();

  showWinner();

}

void showWinner(){

  matrix.setTextSize(2);
  matrix.setCursor(3, 12);
  matrix.print("Player 2 is the winner");

  delay(5000);
  matrix.clear();

}
