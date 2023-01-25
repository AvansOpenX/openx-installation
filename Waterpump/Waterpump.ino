#include <Adafruit_NeoPixel.h>

int motorPinVullen = 3;
int motorPinLegen = 4;

#define waterLed 5

#define waterSensorEmpty 6
#define waterSensorThird 7
#define waterSensorTwoThird 8
#define waterSensorFull 9

int numpixels = 32;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, waterLed, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pinMode(motorPinVullen, OUTPUT);
  pinMode(motorPinLegen, OUTPUT);
  
  pixels.begin();  // This initializes the NeoPixel library.
  pixels.setBrightness(75);

}

void loop() {
  // put your main code here, to run repeatedly:
  int led;
  for (led = 0; led <= numpixels; led++) { //set leds are light blue
    setColor(led, 168, 255, 255, 100);
  }

}

void emptyTank(){

}

void resetTank(){

}

void fillTank(){
  digitalWrite(motorPinLegen, 5);
  int led;
  for (led = 0; led <= numpixels; led++) { //set leds to green
    setColor(led, 50, 205, 50, 100);
  }
  for(int i = 0; i < 5; i++){
    digitalWrite(motorPinVullen, HIGH);
  }
  delay(5000);  
  digitalWrite(motorPinVullen, LOW);
}

