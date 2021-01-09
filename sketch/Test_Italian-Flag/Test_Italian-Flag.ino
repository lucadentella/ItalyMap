#include <Adafruit_NeoPixel.h>

#define NUM_REGIONS 20
#define SIGNAL_PIN  14
Adafruit_NeoPixel strip(NUM_REGIONS, SIGNAL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t orange = strip.Color(255, 60, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t white = strip.Color(255, 255, 255);

void setup() {
  
  strip.begin();
  strip.setBrightness(5);
  strip.clear();
}

void loop() {

  for(int i = 0; i < NUM_REGIONS; i++) {
    strip.setPixelColor(i, orange);
    strip.show();
    delay(50);
  }
  delay(500);
  strip.clear();

  for(int i = NUM_REGIONS - 1; i >= 0; i--) {
    strip.setPixelColor(i, blue);
    strip.show();
    delay(50);
  }
  delay(500);
  strip.clear();


  for(int i = 0; i < 5; i++) strip.setPixelColor(i, green);
  for(int i = 5; i < 12; i++) strip.setPixelColor(i, white);
  for(int i = 12; i < NUM_REGIONS; i++) strip.setPixelColor(i, red); 
  strip.show();
  delay(3000);

  strip.clear();
  delay(500);
}
