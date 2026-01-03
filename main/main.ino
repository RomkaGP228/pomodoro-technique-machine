#include <Arduino.h>
#define KEY_COUNT 2
#define FIRST_KEY_PIN 12
#define FIRST_LED_PIN 24
#define LAST_LED_PIN 42
unsigned long timer1 = 1000 * 60;

void setup() {
  Serial.begin(9600);
  for (int pin = 0; pin < 10; ++pin) {
    pinMode(FIRST_LED_PIN + pin * 2, OUTPUT);
  }
}

void ButtonsLogic() {
  for (int i = 0; i < KEY_COUNT; ++i) {
    int key_pin = FIRST_KEY_PIN + i;
    bool key_up = digitalRead(key_pin);
    if (!key_up) {
      Serial.println("button pressed");
    }
  }
}

void LedLogic() {
  unsigned long int ms = millis();
  int pinn = FIRST_LED_PIN + 2 * ((ms / 100) % 10);
  Serial.println("works");
  digitalWrite(pinn, HIGH);
  delay(5);
  digitalWrite(pinn, LOW);
}

void loop() {
  LedLogic();
}