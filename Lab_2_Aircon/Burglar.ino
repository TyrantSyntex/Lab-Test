#include <Wire.h>
#include "RichShieldTM1637.h"

#define CLK 10
#define DIO 11

#define LDR A2          // Light sensor pin
#define BUTTON 8        // Button 1
#define BUZZER 3        // Buzzer

TM1637 disp(CLK, DIO);

bool armed = false;
bool alarm = false;
int baseLight;

void setup() {
  disp.init();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);

  showOFF();
}

void loop() {
  checkButton();

  if (armed && !alarm)
    detectIntruder();

  if (alarm)
    soundAlarm();
}

void checkButton() {
  static bool lastState = HIGH;

  bool current = digitalRead(BUTTON);

  if (lastState == HIGH && current == LOW) {

    armed = !armed;
    alarm = false;

    if (armed) {
      baseLight = analogRead(LDR);
      showON();
    }
    else {
      digitalWrite(BUZZER, LOW);
      showOFF();
    }

    delay(300);
  }

  lastState = current;
}

void detectIntruder() {
  int light = analogRead(LDR);

  if (abs(light - baseLight) > 80) {
    alarm = true;
  }
}

void soundAlarm() {
  while (alarm) {

    digitalWrite(BUZZER, HIGH);
    delay(200);

    digitalWrite(BUZZER, LOW);
    delay(200);

    if (digitalRead(BUTTON) == LOW) {
      alarm = false;
      armed = false;
      showOFF();
      delay(300);
    }
  }
}

void showON() {
  int8_t data[] = {0, 12, 0, 12};   // Displays "ON" approximately
  disp.display(data);
}

void showOFF() {
  int8_t data[] = {0, 15, 15, 15};  // Simple "OFF" indication
  disp.display(data);
}