/*  List of tests:
  minimum:
    primary:
    1-3
    secondary:
    4-6
    iso test:
    1-4
    3 tests altogether
    1,3,4,6

  extensive:
    primary winding
    1-3
    1-2
    2-3
    secondary winding
    4-6
    4-5
    5-6
    NO continuity between 
    1 - 4
    1 - 5
    1 - 6
    2 - 4
    2 - 5
    2 - 6
    3 - 4
    3 - 5
    3 - 6
    15 tests altogether

  input pins:
  1,2,3,4,5
  output pins:
  3,2,4,5,6
*/
#include <string>
#include <vector>

#include <FastLED.h>
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

// pins in p# format
#define p1 39
//#define p2
#define p3 38
#define p4 36
//#define p5
#define p6 37

// tests in t# format
bool t1 = 0;
bool t2 = 0;
bool t3 = 1; 
// t1,t2,t3 need to be in 1,1,0 to ensure all pins work

std::vector<std::string> failed;

void setup() {
  FastLED.addLeds<WS2812B, 48, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  Serial.begin(115200);
  delay(100);
  // tri-state all pins first
  pinMode(p1, INPUT);
  pinMode(p3, INPUT);
  pinMode(p4, INPUT);
  pinMode(p6, INPUT);

  // Test 1: 1-3
  pinMode(p1, OUTPUT); digitalWrite(p1, LOW);
  pinMode(p3, INPUT_PULLUP); 
  delay(2);
  bool t1 = !digitalRead(p3);
  Serial.println(t1);
  pinMode(p1, INPUT); // release pin
  if (t1 == 0) {failed.push_back("1-3");};

  // Test 2: 1-4
  pinMode(p1, OUTPUT); digitalWrite(p1, LOW);
  pinMode(p4, INPUT_PULLUP); 
  delay(2);
  bool t2 = !digitalRead(p4);
  Serial.println(t2);
  pinMode(p1, INPUT); // release pin
  if (t2 == 0) {failed.push_back("1-4");};

  // Test 3: 3-6
  pinMode(p3, OUTPUT); digitalWrite(p3, LOW);
  pinMode(p6, INPUT_PULLUP); 
  delay(2);
  bool t3 = digitalRead(p6);
  Serial.println(t3);
  pinMode(p3, INPUT); // release pin
  if (t3 == 1) {failed.push_back("3-6");};

  leds[0] = CRGB(0, 255, 0);

  if (failed.empty()) {
    leds[0] = CRGB(0, 255, 0);
    FastLED.show();
    Serial.println("PASS!");
  } else {
    Serial.print("FAIL:");
    for(const std::string& s : failed) {
    Serial.print(" ");
    Serial.print(s.c_str());
  }
  }// could deduct which pins work and which ones dont; if pin 1-3 fails but 1-2 passes, its obvious pin 3 is the issue
}

void loop() {
  // not using to reduce power issues
}
