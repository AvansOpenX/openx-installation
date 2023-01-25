void setColor(int led, int redValue, int greenValue, int blueValue, int delayValue) {
  pixels.setPixelColor(led, pixels.Color(redValue, greenValue, blueValue));
  pixels.show();
  delay(delayValue);
}