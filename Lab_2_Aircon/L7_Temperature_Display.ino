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
  digitalWrite(BUZZER, LOW);
}

void loop() {
  int t = (int)temper.getTemperature();
  displayTemp(t);

  if (t > 28) {
    soundFireAlarm();
  } else {
    digitalWrite(BUZZER, LOW);
    delay(200);
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

// Rising wail: pulses get faster and faster, then resets - like an ambulance siren
void soundFireAlarm() {
  for (int speed = 10; speed >= 1; speed--) {
    digitalWrite(BUZZER, HIGH);
    delay(speed);
    digitalWrite(BUZZER, LOW);
    delay(speed);
  }
}
