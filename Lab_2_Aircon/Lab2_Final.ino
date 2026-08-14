#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldPassiveBuzzer.h"
#include "RichShieldNTC.h"
#include "RichShieldIRremote.h"

#define CLK 10
#define DIO 11
#define BuzzerPin 3
#define NTC_PIN A1
#define LDR_PIN A2
#define VR_PIN A0
#define BUTTON 8
#define LED_BLUE 6
#define RECV_PIN 2
#define LED_GREEN 5
#define LED_YELLOW 7
#define LED_RED 4

#define NOTE_H1 1046
#define NOTE_H5 1568
#define NOTE_M5 784
#define FIRE_THRESHOLD 25 // degrees C, change this if it doesn't trigger right

TM1637 disp(CLK, DIO);
PassiveBuzzer buz(BuzzerPin);
NTC temper(NTC_PIN);
IRrecv IR(RECV_PIN);

int ledPins[3] = {LED_GREEN, LED_YELLOW, LED_RED};
int mode = 1;   // 1 = Night, 2 = Day, 3 = Outside

// burglar system variables
int armed = 0;
int burglarAlarm = 0;
int baseLight = 0;
int lastButtonState = HIGH;
int alarmNotes[] = {NOTE_H1, NOTE_H5, NOTE_H1, NOTE_H5};
int burglarNoteIndex = 0;
unsigned long burglarNoteTimer = 0;
int burglarNoteInterval = 150;

// fire alarm variables
int fireAlarm = 0;
int fireToneHigh = 1;
unsigned long fireToneTimer = 0;
int fireToneInterval = 250;

// aircon variables
int lastKnobValue = 0;
unsigned long knobChangedTime = 0;
int airconValue = 0;

void setup()
{
  Serial.begin(9600);
  disp.init();
  IR.enableIRIn();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_BLUE, OUTPUT);

  for (int i = 0; i < 3; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }

  delay(1000);   // let the temp sensor settle
}

void loop()
{
  checkRemote();
  updateModeLEDs();

  // fire alarm checks in every mode, no matter what
  doFireAlarm();

  // burglar system only runs in Night (1) or Outside (3)
  if (mode == 1 || mode == 3)
  {
    doBurglar();
  }
  else
  {
    armed = 0;
    burglarAlarm = 0;
  }

  // aircon only runs in Night (1) or Day (2)
  if (mode == 1 || mode == 2)
  {
    doAircon();
  }
  else
  {
    digitalWrite(LED_BLUE, LOW);
  }

  updateDisplay();
}

// checks the IR remote and switches mode
void checkRemote()
{
  if (IR.decode())
  {
    if (IR.isReleased())
    {
      if (IR.keycode == 0xC) mode = 1;
      else if (IR.keycode == 0x18) mode = 2;
      else if (IR.keycode == 0x5E) mode = 3;

      Serial.print("mode is now: ");
      Serial.println(mode);
    }
    IR.resume();
  }
}

// turns on the LED for the current mode, turns off the other 2
void updateModeLEDs()
{
  for (int i = 0; i < 3; i++)
  {
    if (i == (mode - 1))
    {
      digitalWrite(ledPins[i], HIGH);
    }
    else
    {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

// checks temperature and sounds fire alarm if too hot
void doFireAlarm()
{
  int temperature = (int)temper.getTemperature();

  if (temperature > FIRE_THRESHOLD)
  {
    fireAlarm = 1;
  }
  else
  {
    fireAlarm = 0;
  }

  if (fireAlarm == 1)
  {
    if (millis() - fireToneTimer >= fireToneInterval)
    {
      fireToneTimer = millis();

      if (fireToneHigh == 1)
      {
        buz.playTone(NOTE_H5, fireToneInterval - 50);
        fireToneHigh = 0;
      }
      else
      {
        buz.playTone(NOTE_M5, fireToneInterval - 50);
        fireToneHigh = 1;
      }
    }
  }
  else
  {
    buz.playTone(0, 0);   // silence, same trick your original fire code used
  }
}

// runs the burglar arm/disarm and intruder check
void doBurglar()
{
  checkButton();

  if (armed == 1 && burglarAlarm == 0)
  {
    checkLight();
  }

  // fire alarm gets the buzzer first if both go off at once
  if (burglarAlarm == 1 && fireAlarm == 0)
  {
    playBurglarAlarm();
  }
}

// checks if the button was just pressed, arms/disarms the system
void checkButton()
{
  int current = digitalRead(BUTTON);

  if (lastButtonState == HIGH && current == LOW)
  {
    if (armed == 0)
    {
      armed = 1;
      baseLight = analogRead(LDR_PIN);
    }
    else
    {
      armed = 0;
      buz.playTone(0, 0);
    }

    burglarAlarm = 0;
    delay(300);   // stops one press counting twice
  }

  lastButtonState = current;
}

// compares current light level to the level when armed
void checkLight()
{
  int currentLight = analogRead(LDR_PIN);
  int difference = currentLight - baseLight;

  if (difference < 0)
  {
    difference = difference * -1;   // make it positive
  }

  if (difference > 80)
  {
    burglarAlarm = 1;
  }
}

// plays the burglar alarm notes on a loop
void playBurglarAlarm()
{
  if (millis() - burglarNoteTimer >= burglarNoteInterval)
  {
    burglarNoteTimer = millis();
    buz.playTone(alarmNotes[burglarNoteIndex], burglarNoteInterval - 30);
    burglarNoteIndex = burglarNoteIndex + 1;

    if (burglarNoteIndex > 3)
    {
      burglarNoteIndex = 0;
    }
  }

  if (digitalRead(BUTTON) == LOW)
  {
    burglarAlarm = 0;
    armed = 0;
    buz.playTone(0, 0);
    delay(300);
  }
}

// reads temp and knob, decides what to show and controls the blue LED
void doAircon()
{
  float celsius = temper.getTemperature();
  int knobValue = analogRead(VR_PIN);
  int threshold = map(knobValue, 0, 1023, 25, 35);

  int knobDifference = knobValue - lastKnobValue;
  if (knobDifference < 0)
  {
    knobDifference = knobDifference * -1;
  }

  if (knobDifference > 15)
  {
    knobChangedTime = millis();
    lastKnobValue = knobValue;
  }

  if (millis() - knobChangedTime < 2000)
  {
    airconValue = threshold;
  }
  else
  {
    airconValue = (int)celsius;
  }

  if (celsius >= threshold)
  {
    digitalWrite(LED_BLUE, HIGH);
  }
  else
  {
    digitalWrite(LED_BLUE, LOW);
  }
}

// decides what the display should show right now
void updateDisplay()
{
  int8_t d[4];

  if (fireAlarm == 1)
  {
    showTemp((int)temper.getTemperature());
  }
  else if (burglarAlarm == 1)
  {
    d[0] = 0; d[1] = 12; d[2] = 0; d[3] = 12;   // shows "ON" while alarming
    disp.display(d);
  }
  else if (mode == 1 || mode == 2)
  {
    showTemp(airconValue);
  }
  else if (mode == 1 || mode == 3)
  {
    if (armed == 1)
    {
      d[0] = 0; d[1] = 12; d[2] = 0; d[3] = 12;   // "ON"
    }
    else
    {
      d[0] = 0; d[1] = 15; d[2] = 15; d[3] = 15;   // "OFF"
    }
    disp.display(d);
  }
}

// shows a temperature number on the display with a C symbol
void showTemp(int temperature)
{
  int8_t d[4];

  if (temperature >= 100)
  {
    d[0] = temperature / 100;
  }
  else
  {
    d[0] = INDEX_BLANK;
  }

  d[1] = (temperature / 10) % 10;
  d[2] = temperature % 10;
  d[3] = 12;   // C symbol

  disp.display(d);
}