//test sketch to have the esp32 send data to the console

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!!");
}

void loop() {
  Serial.println("Hello World!");
  delay(1000);
}
