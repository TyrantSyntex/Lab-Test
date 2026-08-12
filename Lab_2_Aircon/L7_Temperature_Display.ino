#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldNTC.h"

#define NTC_PIN A1
#define CLK 10
#define DIO 11
#define BUZZER 3

NTC temper(NTC_PIN);
TM1637 disp(CLK, DIO);

void setup() {
  disp.init();
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  int t = (int)temper.getTemperature();
  displayTemp(t);

  if (t > 28) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  } else {
    digitalWrite(BUZZER, LOW);
    delay(1000);
  }
}

void displayTemp(int t) {
  int8_t d[4] = {
    (t >= 100) ? t / 100 : INDEX_BLANK,
    (t / 10) % 10,
    t % 10,
    12
  };
  disp.display(d);
}
