//Kaushik
//2621874
//DEEE/FT/1B/22
//Aircon programme

#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldNTC.h"

#define NTC_PIN A1
NTC temper(NTC_PIN);

#define LED_BLUE 6
#define CLK 10
#define DIO 11
TM1637 disp(CLK, DIO);

#define VR_PIN A0

int lastKnobValue = 0;
unsigned long knobChangedTime = 0;

void setup()
{
  disp.init();
  delay(1000);
  pinMode(LED_BLUE, OUTPUT);
}

void loop()
{
  float celsius = temper.getTemperature();
  int knobValue = analogRead(VR_PIN);
  int threshold = map(knobValue, 0, 1023, 25, 35);

  // check how much the knob moved
  int knobDifference = knobValue - lastKnobValue;
  if (knobDifference < 0) {
    knobDifference = knobDifference * -1;  // make it positive
  }

  // if knob moved a lot, remember the time
  if (knobDifference > 15) {
    knobChangedTime = millis();
    lastKnobValue = knobValue;
  }

  // show threshold for 2 seconds after knob is turned, then show real temp
  if (millis() - knobChangedTime < 2000) {
    displayTemperature(threshold, threshold);
  } else {
    displayTemperature((int)celsius, threshold);
  }

  delay(200);
}

void displayTemperature(int temperature, int threshold)
{
  int original = temperature;   // keep the real value for the LED check later

  if (temperature < 0) {
    temperature = temperature * -1;  // make it positive so we can split digits
  }

  int hundreds = temperature / 100;
  int tens = (temperature / 10) % 10;
  int ones = temperature % 10;

  int digits[3];
  digits[0] = hundreds;
  digits[1] = tens;
  digits[2] = ones;

  int8_t temp[4];

  // put the digits into the display array using a loop
  for (int i = 0; i < 3; i++) {
    temp[i] = digits[i];
  }

  if (original < 0) {
    temp[0] = INDEX_NEGATIVE_SIGN;
  } else if (temperature < 100) {
    temp[0] = INDEX_BLANK;
  }

  temp[3] = 12;  // 'C' symbol

  disp.display(temp);

  // turn on LED if temperature has reached the threshold
  if (original >= threshold) {
    digitalWrite(LED_BLUE, HIGH);
  } else {
    digitalWrite(LED_BLUE, LOW);
  }
}
