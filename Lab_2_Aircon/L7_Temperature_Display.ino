#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldNTC.h"

#define NTC_PIN A1
#define CLK 10
#define DIO 11
#define BUZZER 3

NTC temper(NTC_PIN);
TM1637 disp(CLK, DIO);

void setup() 
{
  disp.init();
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}

void loop() 
{
  int temperature = (int)temper.getTemperature();
  displayTemp(temperature);

  if (temperature > 25) 
  {
    soundFireAlarm();
  } 

  else 
  {
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}

// shows the temperature number on the 4-digit display
void displayTemp(int temperature) 
{
  int hundreds;
  if (temperature >= 100) 
  {
    hundreds = temperature / 100;
  } 
  
  else 
  {
    hundreds = INDEX_BLANK;
  }

  int tens = (temperature / 10) % 10;
  int ones = temperature % 10;

  int digits[3];
  digits[0] = hundreds;
  digits[1] = tens;
  digits[2] = ones;

  int8_t d[4];

  // put the digits into the display array using a loop
  for (int i = 0; i < 3; i++) 
  {
    d[i] = digits[i];
  }

  d[3] = 12;   // shows the 'C' symbol
  disp.display(d);
}

// makes a rising siren sound, like an ambulance
// starts slow and speeds up, then repeats
void soundFireAlarm() 
{
  for (int speed = 10; speed >= 1; speed--) 
  {
    digitalWrite(BUZZER, HIGH);
    delay(speed);
    digitalWrite(BUZZER, LOW);
    delay(speed);
  }
}
