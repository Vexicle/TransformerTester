#include <string>
#include <vector>

#include <FastLED.h>
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

// pins in p# format
#define p1 37
#define p2 38
#define p3 39
#define p4 40
#define p5 41
#define p6 42

std::vector<String> failed;

int check(int in, int out) { // in-out format, +, -
  register bool temp = 0;
  pinMode(out, OUTPUT); 
  digitalWrite(out, LOW);
  pinMode(in, INPUT_PULLUP); 
  delay(2);
  temp = !digitalRead(in);
  pinMode(out, INPUT); // release pin

  if (temp == 0) {
    failed.push_back(String(in)); 
    failed.push_back(String(out));
    };
  return temp;
}

void setup() {
  FastLED.addLeds<WS2812B, 48, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  Serial.begin(115200);
  delay(100);
  // tri-state all pins first
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  pinMode(p3, INPUT);
  pinMode(p4, INPUT);
  pinMode(p5, INPUT);
  pinMode(p6, INPUT);

  // -- extensive test
  /* Test 1: 1-2 */ check(p1,p2);
  /* Test 2: 1-3 */ check(p1,p3);
  /* Test 3: 2-3 */ check(p2,p3);

  /* Test 4: 4-5 */ check(p4,p5);
  /* Test 5: 4-6 */ check(p4,p6);
  /* Test 6: 5-6 */ check(p5,p6);

  /* Test 7: 1-4 */ check(p1,p4);
  /* Test 8: 1-5 */ check(p1,p5);
  /* Test 9: 1-6 */ check(p1,p6);

  /* Test 10: 2-4 */ check(p2,p4);
  /* Test 11: 2-5 */ check(p2,p5);
  /* Test 12: 2-6 */ check(p2,p6);

  /* Test 13: 3-4 */ check(p3,p4);
  /* Test 14: 3-5 */ check(p3,p5);
  /* Test 15: 3-6 */ check(p3,p6);


  if (failed.empty()) {
      leds[0] = CRGB(0, 255, 0);
      FastLED.show();
      Serial.println("PASS!");
    } else {
      Serial.print("FAIL:");
      leds[0] = CRGB(255, 0, 0);
      FastLED.show();
      for(const String& s : failed) {
      Serial.print(" ");
      Serial.print(s);
    }
  }// could deduct which pins work and which ones dont; if pin 1-3 fails but 1-2 passes, its obvious pin 3 is the issue
}

void loop() {
  // not using to reduce power issues
}
