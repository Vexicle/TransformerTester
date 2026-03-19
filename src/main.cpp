#include <FastLED.h>
#include <Arduino.h>

#include "init.h"
#include "serial.h"
#include "compute.h"
#include "test.h"

TaskHandle_t SerialHandle = nullptr;
TaskHandle_t ButtonHandle = nullptr;

void setup() {
  Serial.begin(9600);
  init::initialize();
  test::initPins();
  xTaskCreatePinnedToCore(serial::serialMain, "Serial", 10000, nullptr, 1, &SerialHandle, 1);
  //xTaskCreatePinnedToCore(test::btnListener, "Button-Listener", 10000, nullptr, 1, &ButtonHandle, 1);
}

void loop() {
  // use FreeRTOS tasks
}