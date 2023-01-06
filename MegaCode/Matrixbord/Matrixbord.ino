const unsigned int MAX_MESSAGE_LENGHT = 12;

void setup() {
  
  Serial.begin(9600);

}

void loop() {
  
  readESP();


}

void readESP(){

  //kijken of er iets beschikbaar is in de seriële poort
  while(Serial.available() > 0 ){

    //plaats om de seriele communicatie op te slaan
    static char message[MAX_MESSAGE_LENGHT];
    static unsigned int message_pos = 0;

    //eerst volgende byte uitlezen
    char inByte = Serial.read();

    //de byte uitlezen om te kijken of het niet het einde van het bericht is
    if (inByte != '\n' ){

      //byte opslaan in de array
      message[message_pos] = inByte;
      message_pos++;


    } else {

      //bericht is afgelopen 
      message[message_pos] = '\0';

      //print het bericht 
      Serial.println(message);

      //reset array
      message_pos = 0;

    }

  }

}

void showHighscore(){

}

void showTime(){

}

void showScoreP1(){

}

void showScoreP2(){

}
