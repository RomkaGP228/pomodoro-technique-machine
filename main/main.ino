#include <Arduino.h>
#include <LinkedList.h>

// ПИНЫ
#define KEY_COUNT 2
#define FIRST_KEY_PIN 12
#define FIRST_LED_PIN 24
#define LAST_LED_PIN 42

// КОНСТАНТЫ но не тип
unsigned long timer1 = 1000 * 60;
LinkedList<int> low_pins = LinkedList<int>();
int pinn = FIRST_LED_PIN;
void setup() {
  Serial.begin(9600);

  // пиним все диоды
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
    pinMode(pin, OUTPUT);
  }

  // Сей блок нужен чтобы горели с самого начала все диоды
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
    digitalWrite(pin, HIGH);
  }
  delay(1024); // Чтобы в 0 момент горел первый диод
}

int findElement(LinkedList<int> &list, int value) {
  for (int i = 0; i < list.size(); i++) {
    if (list.get(i) == value) {
      return 1; // Нашли! Возвращаем индекс
    }
  }
  return 0; // Не нашли
}

void ButtonsLogic() {
  for (int i = 0; i < KEY_COUNT; ++i) {
    int key_pin = FIRST_KEY_PIN + i * 2;
    bool key_up = digitalRead(key_pin);
    if (!key_up) {
      Serial.println("button pressed");
    }
  }
}

void UpdateLight() {
  for (int pin = 0; pin < 10; ++pin) {
      digitalWrite(FIRST_LED_PIN + 2 * pin, HIGH);
    }
    low_pins.clear();
    pinn = FIRST_LED_PIN;
}

void LedLogic() {
  low_pins.add(pinn);
  digitalWrite(pinn, LOW);
  pinn += 2;
  if (low_pins.size() == 10) {
    delay(1024); // нужно чтобы последний нормально потух
    UpdateLight();
  }
  delay(1024); // нужен чтобы первый загорелся
}

void loop() { 
  LedLogic();
  }