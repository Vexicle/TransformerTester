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

#include <Adafruit_NeoPixel.h>
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

//1-3,1-4,3-6
// need to change 3 from output to input

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

  // Test 2: 1-4
  pinMode(p1, OUTPUT); digitalWrite(p1, LOW);
  pinMode(p4, INPUT_PULLUP); 
  delay(2);
  bool t2 = !digitalRead(p4);
  Serial.println(t2);
  pinMode(p1, INPUT); // release pin

  // Test 3: 3-6
  pinMode(p3, OUTPUT); digitalWrite(p3, LOW);
  pinMode(p6, INPUT_PULLUP); 
  delay(2);
  bool t3 = digitalRead(p6);
  Serial.println(t3);
  pinMode(p3, INPUT); // release pin

  leds[0] = CRGB(0, 255, 0);

  if ((t1==1) && (t2==1)) {
    if (t3==0) {
    leds[0] = CRGB(0, 255, 0);
    FastLED.show();
    Serial.println("pass");
    }
  }
}

void loop() {
  // not using to reduce power issues
}
