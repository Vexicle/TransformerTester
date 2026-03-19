#pragma once

class serial {
  public:
    // Settings
    static bool fastModeEnabled;
    static bool verboseLogging;
    static bool manualModeEnabled;
    static int testCountInt;

    // Functions
    static void serialMain(void *parameter);

    // Commands
    static void help(const String&);
    static void set(const String&);
    static void test(const String&);
};
