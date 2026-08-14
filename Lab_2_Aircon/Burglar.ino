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

int armed = 0;   // 0 = off, 1 = on
int alarm = 0;   // 0 = not alarming, 1 = alarming
int baseLight;    // the light level when we armed the system

#define NOTE_H1 1046
#define NOTE_H5 1568

// the pattern of notes the alarm will play
int alarmNotes[] = {NOTE_H1, NOTE_H5, NOTE_H1, NOTE_H5};
int alarmTempo[] = {4, 4, 4, 4};

void setup() 
{
  disp.init();
  pinMode(BUTTON, INPUT_PULLUP);
  showOFF();
}

void loop() 
{
  checkButton();

  // only check for intruders if we're armed and not already alarming
  if (armed == 1 && alarm == 0) 
  {
    detectIntruder();
  }

  if (alarm == 1) 
  {
    soundAlarm();
  }
}

// checks if the button was just pressed, and switches the alarm on/off
void checkButton() 
{
  static int lastState = HIGH;
  int current = digitalRead(BUTTON);

  if (lastState == HIGH && current == LOW) 
  {

    // flip armed between 0 and 1
    if (armed == 0) 
    {
      armed = 1;
    } 

    else 
    {
      armed = 0;
    }

    alarm = 0;

    if (armed == 1) 
    {
      baseLight = analogRead(LDR);   // remember the current light level
      showON();
    } 

    else 
    {
      showOFF();
    }

    delay(300);   // small pause so one press doesn't count twice
  }

  lastState = current;
}

// checks if the light level has changed a lot since we armed it
void detectIntruder() 
{
  int currentLight = analogRead(LDR);
  int difference = currentLight - baseLight;

  if (difference < 0) 
  {
    difference = difference * -1;   // make it positive
  }

  if (difference > 80) 
  {
    alarm = 1;
  }
}

// plays the alarm sound using the array of notes
void soundAlarm() 
{
  int numberOfNotes = 4;

  for (int i = 0; i < numberOfNotes; i++) 
  {
    if (alarm == 0) 
    {
      break;   // stop early if alarm was turned off
    }

    int noteDuration = 500 / alarmTempo[i];
    buz.playTone(alarmNotes[i], noteDuration);
    delay(noteDuration / 10);

    if (digitalRead(BUTTON) == LOW) 
    {
      alarm = 0;
      armed = 0;
      showOFF();
      delay(300);
    }
  }
}

void showON() 
{
  int8_t d[] = {0, 12, 0, 12};
  disp.display(d);
}

void showOFF() 
{
  int8_t d[] = {0, 15, 15, 15};
  disp.display(d);
}
