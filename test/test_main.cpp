#include "Arduino.h"
#include "ESP8266WiFi.h"

void setup() { Serial.begin(115200); }

void loop() {
  Serial.println(WiFi.SSID().c_str());
  delay(1000);
}