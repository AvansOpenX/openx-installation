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

//scores
int highscore;
int scoreP1 = 346;
int scoreP2 = 502;


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)


void setup() {

  Serial.begin(9600);

  matrix.begin();
  
}


void loop(){

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

//show score if one player played
void showScoreP1(){

  matrix.setTextSize(1);

  matrix.setCursor(8, 4);    // (x, y)
  matrix.print("Player 1:");
  matrix.setTextSize(2);
  matrix.setCursor(12, 14);    // (x, y)
  matrix.print(scoreP1);
  delay(8000);
  matrix.clear();


}

//show score if two players played
void showScoreP2(){

  matrix.setTextSize(1);

  matrix.setCursor(8, 4);    // (x, y)
  matrix.print("Player 2:");
  matrix.setTextSize(2);
  matrix.setCursor(12, 14);    // (x, y)
  matrix.print(scoreP2);
  delay(8000);
  matrix.clear();

}

void showWinner(){

  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setTextSize(2);

  for(int i = 65; i > -364; i--){

    matrix.setCursor(i, 8);
    matrix.print("And the winner is: ...    ");

    if(scoreP1 > scoreP2){
      matrix.print("Player1!!!");
    } else {
      matrix.print("Player2!!!");
    };

    delay(20);
    matrix.clear();

    checkHighscore();

  }


}

void checkHighscore(){

    if(scoreP1 > highscore){
      highscore = scoreP1;
      newHighscore();
  
    } else if (scoreP2 > highscore){
      highscore = scoreP2;
      newHighscore();
    }  

};

void newHighscore(){

  for(int i = 65; i > -100; i--){

    matrix.setCursor(i, 8);
    matrix.print("New HighScore!");

    delay(50);
    matrix.clear();

  };



};

