#pragma once
#include <Arduino.h>

class init {
  public:
  // Variables
  static long start;
  static long current;

  // Variables for ledControl
  static int ledBrightness; // 0-255 control
  static int ledTimeout; // 0-255 control
  static bool ledLock;

  // Functions
  static void initialize();

  static int ledControl(String colour);
};