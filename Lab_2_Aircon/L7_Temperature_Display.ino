#include <Wire.h>
#include "RichShieldTM1637.h"
#include "RichShieldNTC.h"
#include "RichShieldPassiveBuzzer.h" // Added your passive buzzer library

#define NTC_PIN A1
#define CLK 10
#define DIO 11
#define PassiveBuzzerPin 3 // Matches your music code setup

// Define the frequencies for a clean emergency sound
#define NOTE_M5  784
#define NOTE_H5  1568

NTC temper(NTC_PIN);
TM1637 disp(CLK, DIO);
PassiveBuzzer buz(PassiveBuzzerPin); // Initialize passive buzzer

void setup() 
{
  disp.init();
  // The buzzer library handles pin configuration automatically
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
    // Ensure buzzer is quiet when temp is safe
    buz.playTone(0, 0); 
    delay(200);
  }
}

// Shows the temperature number on the 4-digit display
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

  for (int i = 0; i < 3; i++) 
  {
    d[i] = digits[i];
  }

  d[3] = 12;   // Shows the 'C' symbol
  disp.display(d);
}

// Clean, loud, alternating electronic alarm sound
void soundFireAlarm() 
{
  // Play High Tone for 250 milliseconds
  buz.playTone(NOTE_H5, 250);
  delay(75); // Short pause to separate notes cleanly

  // Play Medium-High Tone for 250 milliseconds
  buz.playTone(NOTE_M5, 250);
  delay(75);
}
