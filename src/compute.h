#pragma once
#include <Arduino.h>

class compute {
  public:
  // Functions
  static int levenshtein(const String& a, const String& b);
};