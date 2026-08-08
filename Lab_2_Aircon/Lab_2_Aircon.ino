//Kaushik
//2621874
//DEEE/FT/1B/22
//Aircon programme
//Tarun Kumar
//P2603616
//DEEE/FT/1B/22

#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldNTC.h"
#define NTC_PIN A1 //SIG pin of NTC module connect to A1 of IO Shield
NTC temper(NTC_PIN);  
#define LED_BLUE 6
#define CLK 10//CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11//DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3
TM1637 disp(CLK,DIO);
#define VR_PIN A0   // blue knob/potentiometer

void setup()
{
  disp.init();
  delay(1000);
  pinMode(LED_BLUE, OUTPUT);
}

void loop()
{
  float celsius;
  celsius = temper.getTemperature();

  int threshold = map(analogRead(VR_PIN), 0, 1023, 25, 35);  // knob sets 25-35°C

  displayTemperature((int8_t)celsius, threshold);
  delay(1000);
}

void displayTemperature(int8_t temperature, int threshold)
{
  int8_t original = temperature;   // save the real value before we chop it up for display
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
  temp[3] = 12;  // 'C' symbol
  disp.display(temp);

  if(original >= threshold)
  {
    digitalWrite(LED_BLUE, HIGH);   // too hot based on knob setting -> aircon ON
  }
  else
  {
    digitalWrite(LED_BLUE, LOW);    // below threshold -> aircon OFF
  }
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/

