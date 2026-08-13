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
TM1637 disp(CLK,DIO);
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

  int knobDifference = knobValue - lastKnobValue;
  if (knobDifference < 0) {
    knobDifference = -knobDifference;
  }

  if (knobDifference > 15) {
    knobChangedTime = millis();
    lastKnobValue = knobValue;
  }

  if (millis() - knobChangedTime < 2000) {
    displayTemperature((int8_t)threshold, threshold);
  } else {
    displayTemperature((int8_t)celsius, threshold);
  }

  delay(200);
}

void displayTemperature(int8_t temperature, int threshold)
{
  int8_t original = temperature;
  int8_t temp[4];
  bool negative = (temperature < 0);
  if (negative) temperature = abs(temperature);

  int digits[3] = { temperature / 100, (temperature / 10) % 10, temperature % 10 };

  // Loop through the digits array to fill the display array
  for (int i = 0; i < 3; i++) {
    temp[i] = digits[i];
  }

  if (negative) temp[0] = INDEX_NEGATIVE_SIGN;
  else if (temperature < 100) temp[0] = INDEX_BLANK;

  temp[3] = 12;  // 'C' symbol
  disp.display(temp);

  if (original >= threshold) {
    digitalWrite(LED_BLUE, HIGH);
  } else {
    digitalWrite(LED_BLUE, LOW);
  }
}
