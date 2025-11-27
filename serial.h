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

  static void serial::placeholder(const String& input);
};
