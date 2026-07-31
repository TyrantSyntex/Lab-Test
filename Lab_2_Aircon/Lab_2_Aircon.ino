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

#define NTC_PIN A1 //SIG pin of NTC module cornnect to A1 of IO Shield, that is pin A1 of OPEN-SMART UNO R3
NTC temper(NTC_PIN);  

#define LED_BLUE 6
#define CLK 10//CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11//DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3
TM1637 disp(CLK,DIO);

void setup()
{
  disp.init();//The initialization of the display
  delay(1000);//

	pinMode(LED_BLUE, OUTPUT);
}

void loop()
{
  float celsius;
  celsius = temper.getTemperature();//get temperature
  displayTemperature((int8_t)celsius);//
  delay(1000);//delay 1000ms
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

void displayTemperature(int8_t temperature)
{
  int8_t temp[4];
  if(temperature < 0)
	{
		temp[0] = INDEX_NEGATIVE_SIGN;
		temperature = abs(temperature);
	}
	else if(temperature < 100)temp[0] = INDEX_BLANK;
	else temp[0] = temperature/100;
	temperature %= 100;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);

	if(temperature <= 18)
	{
   digitalWrite(LED_BLUE, HIGH);
	}
	else if (temperature >= 27)
	{
		digitalWrite(LED_BLUE, HIGH);
	}
	else
	{
		digitalWrite(LED_BLUE, LOW);
	}
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/

