#include "test.h"
#include <Arduino.h>
#include <init.h>
#include <vector>
#include <string>

#define p1 37
#define p2 38
#define p3 39
#define p4 40
#define p5 41
#define p6 42

#define btn 10

void test::initPins() { // trick the compiler
  pinMode(p1, INPUT_PULLUP);
  pinMode(p2, INPUT_PULLUP);
  pinMode(p3, INPUT_PULLUP);
  pinMode(p4, INPUT_PULLUP);
  pinMode(p5, INPUT_PULLUP);
  pinMode(p6, INPUT_PULLUP);

  pinMode(10, INPUT_PULLUP);
}

bool test::testLock = 0;
bool btnLock = 0;

std::vector<int> failed;
std::vector<int> passed;
std::vector<int> final;

int check(int in, int out) {
    bool temp = 0;
    pinMode(out, OUTPUT);
    digitalWrite(out, LOW);
    pinMode(in, INPUT_PULLUP);
    delay(2);
    temp = !digitalRead(in);
    pinMode(out, INPUT);

    if (temp == 0) {
        if (std::find(failed.begin(), failed.end(), in) == failed.end())
            failed.push_back(in);
        if (std::find(failed.begin(), failed.end(), out) == failed.end())
            failed.push_back(out);
    } else {
        if (std::find(passed.begin(), passed.end(), in) == passed.end())
            passed.push_back(in);
        if (std::find(passed.begin(), passed.end(), out) == passed.end())
            passed.push_back(out);
    }

    return temp;
}

String translate(int pin) {
  switch(pin) {
        case 37: return "p1";
        case 38: return "p2";
        case 39: return "p3";
        case 40: return "p4";
        case 41: return "p5";
        case 42: return "p6";
        default: return String(pin);
    }
}

String test::run(int type) { // 0 is simple pass fail, 1 is detailed for serial



  if (testLock == 1) {
    init::ledControl("y");
    return "Denied";
  }

  testLock = 1;
  failed.clear();
  passed.clear();
  final.clear();

  check(p1,p2);
  check(p1,p3);
  check(p2,p3);

  check(p4,p5);
  check(p4,p6);
  check(p5,p6);

  check(p1,p4);
  check(p1,p5);
  check(p1,p6);

  check(p2,p4);
  check(p2,p5);
  check(p2,p6);

  check(p3,p4);
  check(p3,p5);
  check(p3,p6);

  for (const int& f : failed) { // pins that failed but passed other tests means they are functional
    if (std::find(passed.begin(), passed.end(), f) == passed.end()) {
        final.push_back(f);
    }
  }

  String result;

  if (type == 0){
    if (final.empty()) {// pass
      init::ledControl("g");
      result = "1";
    } else { // fail
      init::ledControl("r");
      result = "0";
    };
  } else {
      if (final.empty()) {
      result = "PASS!";
    } else {
      result = "FAIL: ";
      for (const int& s : final) {
        result += translate(s) + " ";
      }
    }
  }

  testLock = 0;
  return result;
}

void test::btnListener(void *parameter) {
    while (true) {
        if (digitalRead(btn) == HIGH) {
            Serial.println("press!");
            btnLock = 1;
            if (test::testLock == 0) {
                if (test::run(0) == "0") {
                    init::ledControl("g");
                    Serial.println("g");
                    btnLock = 0;
                }
            } else {
                init::ledControl("y");
                Serial.println("y");
                btnLock = 0;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); // small delay to yield CPU
    }
}
