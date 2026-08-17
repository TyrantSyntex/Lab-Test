//Kaushik
//2621874
//DEEE/FT/1B/22
//Aircon programme

#include <Wire.h>
#include "RichShieldTM1637.h" // library for 7 segment
#include "RichShieldNTC.h" // library for thermometer

#define NTC_PIN A1
NTC temper(NTC_PIN);

#define LED_BLUE 6
#define CLK 10
#define DIO 11
TM1637 disp(CLK, DIO);

#define VR_PIN A0 

int lastKnobValue = 0; //stores last to compare
unsigned long knobChangedTime = 0; // when it was last moved

void setup()
{
  disp.init();
  delay(1000); // time for display hardware to settle
  pinMode(LED_BLUE, OUTPUT);
}

void loop()
{
  float celsius = temper.getTemperature(); // read it as a decimal number
  int knobValue = analogRead(VR_PIN); // reads position of knob
  int threshold = map(knobValue, 0, 1023, 25, 35); // rescale a number from 25- 35

  int knobDifference = knobValue - lastKnobValue; // how much moved since last reading
  if (knobDifference < 0) 
  {
    knobDifference = knobDifference * -1;
  }

  if (knobDifference > 15) // treated as real delibrate knob adjustment
  {
    knobChangedTime = millis();
    lastKnobValue = knobValue;
  }

  int valueToDisplay;
  if (millis() - knobChangedTime < 2000)  // shows the value of knob adjusted to for 2 seconds
  {
    valueToDisplay = threshold;
  } 
  
  else 
  {
    valueToDisplay = (int)celsius; //else go back to showing current temperature
  }

  displayTemperature(valueToDisplay, threshold); // update value

  delay(200); // waits bedore loop repeats
}

// Criterion i: At least 1 Custom Function
void displayTemperature(int temperature, int threshold)
{
  int tens = (temperature / 10) % 10; //ones digit
  int ones = temperature % 10; //tens digit

  // Step 1: Create a raw array of the individual digits
  int rawDigits[4];
  rawDigits[0] = INDEX_BLANK; // Position 0: Blank space
  rawDigits[1] = tens;         // Position 1: Tens digit
  rawDigits[2] = ones;         // Position 2: Ones digit
  rawDigits[3] = 12;           // Position 3: 'C' symbol

  // Criterion ii: Array with a loop doing real work
  // Step 2: Use the loop to copy and process elements from rawDigits into the final display array
  int8_t temp[4];
  for (int i = 0; i < 4; i++)
  {
    temp[i] = (int8_t)rawDigits[i]; // converts 4 elements into 8bit integer
  }

  // Send the final array to the screen module
  disp.display(temp);

  // Criterion iii: Embedded System I/O
  float realTemp = temper.getTemperature(); // takes fresh temperature reading
  if (realTemp >= threshold) 
  {
    digitalWrite(LED_BLUE, HIGH);
  } 
  
  else 
  {
    digitalWrite(LED_BLUE, LOW);
  }
}
