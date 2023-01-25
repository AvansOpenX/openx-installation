#include <Adafruit_NeoPixel.h>

#define PIN 8

int numpixels = 24;
int value;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();  // This initializes the NeoPixel library.
  pixels.setBrightness(75);
}

int kleur;
int led;
int ledjes_aan = 24;
int red, green, blue = 0;

void loop() {
  for (led = 0; led <= led; led++) {

    if (ledjes_aan <= 6) {
      // wordt rood
      red = 255;
      green = 0;
      blue = 0;
    } else if (ledjes_aan >= 7 && ledjes_aan <= 12) {
      // wordt oranje
      green = 100;
      red = 255;
      blue = 0;
    } else if (ledjes_aan >= 13 && ledjes_aan <= 18) {
      // wordt geel
      green = 200;
      red = 255;
      blue = 0;
    } else {
      // wordt groen
      green = 255;
      red = 0;
      blue = 0;
    }

    led = constrain(led, 0, ledjes_aan);
    Serial.println(led);

    setColor(led, red, green, blue, 100);
  }
}
