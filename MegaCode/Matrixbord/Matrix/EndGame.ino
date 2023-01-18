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