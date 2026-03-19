#pragma once
#include <Arduino.h>
#include <string>

class test {
  public:
    // Variables for tester
    static bool testLock;

    // Functions
    static void initPins();
    static String run(int type);

    // button listener
    static void btnListener(void *parameter);
};