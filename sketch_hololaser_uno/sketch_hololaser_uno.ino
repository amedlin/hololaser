
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
const int laserPin = 4;

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int lastButtonState = buttonState;
int ledState = LOW;

unsigned long lastTime;

// enum SystemState
// {
//   UNDEFINED = 0,
//   WAITING_INPUT,
//   COUNTDOWN,
//   LASER_WARMUP,
//   LASER_OPENING,
//   LASER_EXPOSURE,
//   LASER_OFF,
//   LASER_CLOSING
// };

// SystemState systemState = UNDEFINED;

void setup() 
{
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(laserPin, OUTPUT);

  lastTime = millis();
}

void loop() 
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState && millis() > lastTime + 500)
  {
    lastTime = millis();
    
    if (ledState == HIGH) 
    {
      // turn LED off:
      ledState = LOW;
    }
    else
    {
      // turn LED on:
      ledState = HIGH;
    }
//    digitalWrite(ledPin, ledState);
    digitalWrite(laserPin, ledState);
  }

  lastButtonState = buttonState;
}
