#pragma once

#include <Arduino.h>

class serial { // class conflict?
  public:
  // Settings
  static bool fastModeEnabled;
  static bool verboseLogging;
  static bool manualModeEnabled;
  static int testCountInt;

  // Functions
  static void serial::serialMain(void *parameter);

  // Commands
  static void serial::help();
  static void serial::set(String input);

  // Settings
  static void serial::fastMode(bool state);
  static void serial::verbose(bool state);
  static void serial::manualMode(bool state);
  static void serial::testCount(int value);
};
