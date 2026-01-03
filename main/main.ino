#include <Arduino.h>
#define KEY_COUNT 2
#define FIRST_KEY_PIN 40

void setup() { Serial.begin(9600); }

void loop() {
  for (int i = 0; i < KEY_COUNT; ++i) {
    int key_pin = FIRST_KEY_PIN + i * 2;
    bool key_up = digitalRead(key_pin);
    if (!key_up) {
      Serial.println("button pressed");
    }
  }
}