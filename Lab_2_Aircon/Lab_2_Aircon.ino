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

int lastKnobValue = 0;         // remembers the knob's last reading
unsigned long knobChangedTime = 0;   // remembers WHEN the knob was last turned

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

  // Check if knob moved since last time
  int knobDifference = knobValue - lastKnobValue;
  if (knobDifference < 0) {
    knobDifference = -knobDifference;   // make it positive, same as abs()
  }

  if (knobDifference > 15) {
    knobChangedTime = millis();     // record the time it was turned
    lastKnobValue = knobValue;      // update last known knob position
  }

  // If it's been less than 2000ms (2 sec) since the knob was turned, show threshold
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
  if(temperature < 0)
  {
    temp[0] = INDEX_NEGATIVE_SIGN;
    temperature = abs(temperature);
  }
  else if(temperature < 100) temp[0] = INDEX_BLANK;
  else temp[0] = temperature/100;
  temperature %= 100;
  temp[1] = temperature / 10;
  temp[2] = temperature % 10;
  temp[3] = 12;
  disp.display(temp);

  if(original >= threshold)
  {
    digitalWrite(LED_BLUE, HIGH);
  }
  else
  {
    digitalWrite(LED_BLUE, LOW);
  }
}

