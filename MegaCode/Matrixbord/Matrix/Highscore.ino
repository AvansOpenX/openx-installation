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