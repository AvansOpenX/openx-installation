void showWaterPlants(){

  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setTextSize(2);

  for(int i = 65; i > -364; i--){

    matrix.setCursor(i, 8);
    matrix.print("Play the game to give us water!");

    delay(20);
    matrix.clear();


  }


}