#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldPassiveBuzzer.h"

#define CLK 10
#define DIO 11
#define LDR A2
#define BUTTON 8
#define BuzzerPin 3

TM1637 disp(CLK, DIO);
PassiveBuzzer buz(BuzzerPin);

bool armed = false, alarm = false;
int baseLight;

#define NOTE_H1  1046
#define NOTE_H5  1568

// Alarm pattern: alternating high notes
const static int alarmNotes[] = {
  NOTE_H1, NOTE_H5, NOTE_H1, NOTE_H5
};
const static int alarmTempo[] = {
  4, 4, 4, 4
};

void setup() {
  disp.init();
  pinMode(BUTTON, INPUT_PULLUP);
  showOFF();
}

void loop() {
  checkButton();
  if (armed && !alarm) detectIntruder();
  if (alarm) soundAlarm();
}

void checkButton() {
  static bool lastState = HIGH;
  bool current = digitalRead(BUTTON);
  if (lastState == HIGH && current == LOW) {
    armed = !armed;
    alarm = false;
    if (armed) { baseLight = analogRead(LDR); showON(); }
    else { showOFF(); }
    delay(300);
  }
  lastState = current;
}

void detectIntruder() {
  if (abs(analogRead(LDR) - baseLight) > 80) alarm = true;
}

void soundAlarm() {
  int size = sizeof(alarmNotes) / sizeof(int);
  for (int i = 0; i < size && alarm; i++) {
    int noteDuration = 500 / alarmTempo[i];
    buz.playTone(alarmNotes[i], noteDuration);
    delay(noteDuration * 0.1);

    if (digitalRead(BUTTON) == LOW) {
      alarm = false;
      armed = false;
      showOFF();
      delay(300);
    }
  }
}

void showON()  { int8_t d[] = {0, 12, 0, 12};  disp.display(d); }
void showOFF() { int8_t d[] = {0, 15, 15, 15}; disp.display(d); }
