#include <Adafruit_NeoPixel.h>

#define PIN 8

int numPixels = 32;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPixels, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();  // This initializes the NeoPixel library.
  pixels.setBrightness(100);
}

void loop() {
  for (int led = 0; led <= numPixels; led++) {

    setColor(led, 255, 255, 255, 100);
  }
  delay(100);
  for (int led = 0; led <= numPixels; led++) {

    setColor(led, 0, 0, 0, 100);
  }
  delay(100);
}
