#define BUTTON_K2 9
#define LED_GREEN 5
#define LED_YELLOW 7
#define LED_RED 4
int mode = 1;


void setup() 
{
  // put your setup code here, to run once:
  pinMode(BUTTON_K2, INPUT_PULLUP);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON_K2) == LOW) {
    mode = mode + 1;

    if (mode == 4) 
    {
      mode = 1;
    }

    delay(300);
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
