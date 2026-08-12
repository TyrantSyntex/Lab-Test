#include <Wire.h>
#include "RichShieldIRremote.h"

// ---- IR remote ----
#define RECV_PIN 2
IRrecv IR(RECV_PIN);

// ---- Mode LEDs ----
#define LED_GREEN 5
#define LED_YELLOW 7
#define LED_RED 4
int mode = 1;

void setup()
{
  Serial.begin(9600);
  IR.enableIRIn();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void loop()
{
  // Check remote for mode change
  if (IR.decode()) {
    if (IR.isReleased()) {
      if (IR.keycode == 0xC) mode = 1;         // Button 1 -> Night Mode
      else if (IR.keycode == 0x18) mode = 2;   // Button 2 -> Day Mode
      else if (IR.keycode == 0x5E) mode = 3;   // Button 3 -> Outside Mode

      Serial.print("mode is now: ");
      Serial.println(mode);
    }
    IR.resume();
  }

  // Night Mode
  if (mode == 1) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    // Put Night Mode code here
  }

  // Day Mode
  if (mode == 2) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    // Put Day Mode code here
  }

  // Outside Mode
  if (mode == 3) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    // Put Outside Mode code here
  }
}
