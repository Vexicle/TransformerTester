/*  UNO R3 specs
 *  
 *  GPIO, UART, I2C, SPI, PWM, ADC
 *  5v 20mA per I/O pin
 *  8-bit AVR 16mhz
 *  2KB SRAM, 32KB FLASH, 1kb EEPROM
 */

#include <Arduino.h>
#include <Arduino_FreeRTOS.h> // install FreeRTOS by Richard Barry in library manager

#include "init.h"
#include "serial.h"

TaskHandle_t SerialHandle = NULL;

void setup() {
  Serial.begin(115200);
  init::initialize();


  // Serial could use the most bytes; not much ram required for testing the IC
  xTaskCreate(serial::serialMain, "Serial", 128, NULL, 1, &SerialHandle);
  vTaskStartScheduler();
}

void loop() {
  // use FreeRTOS tasks
}
