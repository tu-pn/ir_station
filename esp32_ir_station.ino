/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is based on the Arduino WiFi Shield library, but has significant changes as newer WiFi functions are supported.
 *  E.g. the return value of `encryptionType()` different because more modern encryption is supported.
 */
#include "src/app_wifi.h"

#define SERIAL_DEFAILT_BAUDRATE 115200

void setup() {
  Serial.begin(SERIAL_DEFAILT_BAUDRATE);

  wifiStart();

  Serial.println("Setup done");
}

void loop() {
  Serial.println("Scan start");

  // wifiScan();

  // Wait a bit before scanning again.
  delay(5000);
}
