#include <Wire.h>
#include <Wire.h>
#include "RichShieldIRremote.h"

#define RECV_PIN 2
IRrecv IR(RECV_PIN); //tells which pin to listen to

#define LED_GREEN 5
#define LED_YELLOW 7
#define LED_RED 4
int ledPins[3] = {LED_GREEN, LED_YELLOW, LED_RED};   // array of LED pins
int mode = 1; // to track mode

void setup()
{
  Serial.begin(9600);
  IR.enableIRIn(); // tells it to start listening

  // Loop through array to set all 3 LEDs as OUTPUT
  for (int i = 0; i < 3; i++) 
  {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop()
{
  if (IR.decode()) //checks if the condition is true to proceed
  {
    if (IR.isReleased()) // sees if it is released so that when someone holds it, it does not keep repeating
    {
      if (IR.keycode == 0xC) mode = 1;
      else if (IR.keycode == 0x18) mode = 2;  // to check which button is pressed 
      else if (IR.keycode == 0x5E) mode = 3;

      Serial.print("mode is now: ");
      Serial.println(mode); // print the mode into serial monitor for tracking
    }
    IR.resume();
  }

  updateModeLEDs();

  if (mode == 1) 
  {
    // Put Night Mode code here
  }
  if (mode == 2) 
  {
    // Put Day Mode code here
  }
  if (mode == 3) 
  {
    // Put Outside Mode code here
  }
}

// Function: turns on only the LED matching current mode, turns off the rest
void updateModeLEDs()
{
  for (int i = 0; i < 3; i++) // to show LED based on the mode youre on
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
