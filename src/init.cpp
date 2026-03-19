#include <FastLED.h>
#include <Arduino.h>
#include "init.h"

// pins in p# format

// LED pin
#define ledPin 48

#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

long init::start;
long init::current;

int init::ledBrightness = 200; // 0-255 control
int init::ledTimeout = 4; // 0-255 control
bool init::ledLock = 0;

int init::ledControl(String colour) {
  if (colour == "r") {
    leds[0] = CRGB(255, 0, 0);
    FastLED.show();
  }else if (colour == "g") {
    leds[0] = CRGB(0, 255, 0);
    FastLED.show();
  } else {
    leds[0] = CRGB(255, 255, 0);
    FastLED.show();
  }
}

void init::initialize() {
  init::start = millis();
};