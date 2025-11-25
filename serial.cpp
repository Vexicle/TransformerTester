// TTL implementation prototype
#include <Arduino_FreeRTOS.h> // double include?
#include "serial.h"

// defaults
bool fastModeEnabled = false;
bool verboseLogging = false;
bool manualModeEnabled = false;
int testCountInt = 3;

enum SettingType {BOOL,INT};

struct Command {
  const char* name;
  const char* description;
};

struct Setting {
  const char* name;
  SettingType type;
  void* value;
  const char* description;
};

// dynamic settings and commands list
Setting settings[] = {
  {"fastmode", BOOL, &fastModeEnabled, "No clue"},
  {"verbose",  BOOL, &verboseLogging, "Verbose debug logging"},
  {"manual",   BOOL, &manualModeEnabled, "Manually test the IC via command"},
  {"testcount", INT, &testCountInt, "Number of tests to run per IC"}, // might be useless
};

Command commands[] = {
  {"test", "Execute this command to test the IC"},
};

void serialMain(void *parameter) {
  while (true) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n'); input.trim();  // clean whitespace

      if (input == "help") {
        help();
      } else if (input.startsWith("set ")) { // set <key> <value>
        String arg = input.substring(4);
        int seperator = arg.indexOf(' ');
        if (seperator < 0) {
          Serial.println("Usage: <setting> <value>");
        }
        String key = arg.substring(0, seperator);
        String value = arg.substring(seperator + 1);
        // todo: go through settings table names, if key matches a function on the list, change the corresponding value
        /* optimization idea: could scan first 3 characters of functions to check whether they'd match, then check if key is actually a real setting
         * might save on some memory, or slight speed increase
         * first 3 characters lookup table needs to be automatically generated
         */
      }
    }
    vTaskDelay(1);  // yield
  }
}


void help() {
  Serial.println("--- Commands ---");
  for (size_t i = 0; i < sizeof(commands)/sizeof(Command); i++) {
    Serial.println(commands[i].name);
    Serial.print("Description: ");
    Serial.println(commands[i].description);
  }
  Serial.println("--- Settings ---");
  Serial.println("Usage: set [setting] value");
  Serial.println("Hint: Current settings are displayed");

  for (size_t i = 0; i < sizeof(settings)/sizeof(Setting); i++) {
      Serial.print(settings[i].name);
      Serial.print(" ");
      if (settings[i].type == BOOL) {
        bool val = *(bool*)settings[i].value;
        Serial.println(val ? "true" : "false");

      } else if (settings[i].type == INT) {
          int val = *(int*)settings[i].value;
          Serial.println(val);
      }

      Serial.print("Description: ");
      Serial.println(settings[i].description);
  }
}

void fastMode(bool state) { fastModeEnabled = state;
  // do tests faster
}

void verbose(bool state) { verboseLogging = state;
  // all we have to do is just check if this is enabled in the main loop
}

void manualMode(bool state) { manualModeEnabled = state;
  // command test only; or button press
}

void testCount(int value) { testCountInt = value;
  // do multiple tests; check if any of them failed; could false positive though
}
