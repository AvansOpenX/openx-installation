#include <Adafruit_NeoPixel.h>

#define PIN 5

int numpixels = 32;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();  // This initializes the NeoPixel library.
  pixels.setBrightness(75);
}

void loop() {
  int led;
  for (led = 0; led <= numpixels; led++) {
    setColor(led, 168, 255, 255, 100);
  }
}

//simple function which takes values for the red, green and blue led and also
//a delay
void setColor(int led, int redValue, int greenValue, int blueValue, int delayValue) {
  pixels.setPixelColor(led, pixels.Color(redValue, greenValue, blueValue));
  pixels.show();
  delay(delayValue);
}