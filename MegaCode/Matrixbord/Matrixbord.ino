#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h> //for the fonts
#include <Fonts/FreeSerif9pt7b.h>


#define CLK 11   // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3


// Enable double buffering
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)

void setup() {
  Serial.begin(9600); //Pin 0 en 1
  Serial1.begin(9600);
  matrix.begin();

}

void loop() {
  matrix.setCursor(3, 6);    // (x, y)
  String tekst = Serial1.readString();
  matrix.print(tekst);
  delay(1000);
  matrix.clear();
}

