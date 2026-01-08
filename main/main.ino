#include <Arduino.h>
#include <LinkedList.h>

// ПИНЫ
#define KEY_COUNT 2
#define STOP_BUTTON 12
#define RESET_BUTTON 13
#define FIRST_LED_PIN 24
#define LAST_LED_PIN 42
#define BUZZER 11
#define RED_LED 5
#define GREEN_LED 6

// КОНСТАНТЫ но не тип
unsigned long timer1 = 1000 * 60;
LinkedList<int> low_pins = LinkedList<int>();
int pinn = FIRST_LED_PIN;
bool stop_flag = 1;

unsigned long time_delay = 1024 * 6;

bool stop_button_was_up = 1;
bool reset_button_was_up = 1;

unsigned long last_led_update_time = 0;
bool is_waiting_reset = false;

bool chill_flag = 1;

void setup() {
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  // пиним все диоды
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
    pinMode(pin, OUTPUT);
  }

  // Сей блок нужен чтобы горели с самого начала все диоды
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
    digitalWrite(pin, HIGH);
  }
}

void loop() {
  if (!stop_flag) {
    LedLogic();
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
  } else {
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
  }
  StopButtonLogic();
  ResetButtonLogic();
}

int findElement(LinkedList<int> &list, int value) {
  for (int i = 0; i < list.size(); i++) {
    if (list.get(i) == value) {
      return 1; // Нашли! Возвращаем индекс
    }
  }
  return 0; // Не нашли
}

void StopButtonLogic() {
  bool stop_button_is_up = digitalRead(12);
  if ((!stop_button_is_up) && stop_button_was_up) {
    stop_button_is_up = digitalRead(12);
    if (!stop_button_is_up) {
      if (stop_flag) {
        stop_flag = 0;
      } else {
        stop_flag = 1;
      }
      Serial.println("button pressed");
      Serial.println(stop_flag);
    }
  }
  stop_button_was_up = stop_button_is_up;
}

void BuzzerLogic() {
  tone(BUZZER, 4000, 100);
  delay(1000);
  tone(BUZZER, 4000, 100);
  delay(1000);
  tone(BUZZER, 4000, 100);
  delay(1000);
}

void ResetButtonLogic() {
  bool reset_button_is_up = digitalRead(13);
  if ((!reset_button_is_up) && reset_button_was_up) {
    reset_button_is_up = digitalRead(13);
    if (!reset_button_is_up) {
      for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
        digitalWrite(pin, HIGH);
      }
      pinn = FIRST_LED_PIN;
      low_pins.clear();
      stop_flag = 1;
    }
  }
  reset_button_was_up = reset_button_is_up;
}

void UpdateLight() {
  for (int pin = 0; pin < 10; ++pin) {
    digitalWrite(FIRST_LED_PIN + 2 * pin, HIGH);
  }
  low_pins.clear();
  pinn = FIRST_LED_PIN;
  if (chill_flag) {
    time_delay /= 2;
    chill_flag = 0;
  } else {
    time_delay = 1024 * 6;
    chill_flag = 1;
  }
}

void LedLogic() {
  if (millis() - last_led_update_time >= time_delay) {
    last_led_update_time = millis();

    if (is_waiting_reset) {
      // После буззера зажигаем все
      for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
        digitalWrite(pin, HIGH);
      }
      low_pins.clear();
      pinn = FIRST_LED_PIN;
      if (chill_flag) {
        time_delay /= 2;
        chill_flag = 0;
      } else {
        time_delay = 1024 * 6;
        chill_flag = 1;
      }
      is_waiting_reset = false;
      return;
    }

    low_pins.add(pinn);
    digitalWrite(pinn, LOW);
    pinn += 2;

    if (low_pins.size() == 10) {
      for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; pin += 2) {
        digitalWrite(pin, LOW);
      }
      is_waiting_reset = true;
      BuzzerLogic();
      return;
    }
  }
}
