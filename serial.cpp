// TTL implementation prototype
#include <Arduino_FreeRTOS.h> // double include?
#include "serial.h"
#include "compute.h"

// defaults
bool fastModeEnabled = false;
bool verboseLogging = false;
bool manualModeEnabled = false;
int testCountInt = 3;

enum SettingType {BOOL,INT};

struct Command {
  const char* name;
  void (*function)(const String&); // nice
  const char* description;
  const char* usage;
};

struct Setting {
  const char* name;
  SettingType type;
  void* value;
  const char* description;
};

// dynamic settings list
Setting settings[] = { // setting, datatype, variable, description
  {"fastmode", BOOL, &fastModeEnabled, "No clue"},
  {"verbose",  BOOL, &verboseLogging, "Verbose debug logging"},
  {"manual",   BOOL, &manualModeEnabled, "Manually test the IC via command"},
  {"testcount", INT, &testCountInt, "Number of tests to run per IC"}, // might be useless
};

Command commands[] = { // command, function, description, usage
  {"test", serial::placeholder, "Execute this command to test the IC", "test"}, // change with real function
  {"help", serial::help, "Show this help table", "help"},
  {"set", serial::set, "Change the settings of the tester", "set <setting> <value>"},
};
const int commandCount = sizeof(commands) / sizeof(commands[0]);

void serial::serialMain(void *parameter) {
  while (true) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n'); input.trim();  // clean whitespace
      if (input.length() == 0)
        continue;

        int firstSpace = input.indexOf(' ');
        String cmd = (firstSpace == -1) ? input : input.substring(0, firstSpace);

        bool matched = false;

        for (int i = 0; i < commandCount; i++) {
          if (cmd.equalsIgnoreCase(commands[i].name)) {
            commands[i].function(input);     // execute command
            matched = true;
            break;
          }
        }
      if (matched == false) { // autocomplete typos
        int assumedDist = 0xff; // any big number
        String assumedCmd = "";

        for (int i = 0; i < commandCount; i++) {
          int d = compute::levenshtein(cmd, commands[i].name);
            if (d < assumedDist) {
              assumedDist = d;
              assumedCmd = commands[i].name;
            }
          }

          if (assumedDist <= 2) {   // format off linux, "command not found: x, did you mean xy?"
            Serial.print("command not found: ");
            Serial.print(cmd);
            Serial.print(", did you mean: ");
            Serial.print(assumedCmd);
            Serial.println("?");
          } else { // "command not found: x, type 'help' for a list of commands."
            Serial.print("command not found: ");
            Serial.print(cmd);
            Serial.print(", type 'help' for a list of commands.");
          }

      }
    }
    vTaskDelay(1);  // yield
  }
}

void serial::help() {
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

void serial::set(String input) {
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

void serial::placeholder(const String& input) {} // function placeholder
