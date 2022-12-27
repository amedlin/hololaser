#include <Servo.h>

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
const int laserPin = 4;
const int servoPwmPin = 11;

// Input variables
int buttonState = 0;  // variable for reading the pushbutton status
int potLevel = 0; // potentiometer position

// Global state
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
  pinMode(ledPin, OUTPUT); // Even though we're not using this pin, I found for my board I need to do this to force the inbuilt LED to be properly off.
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // Laser power outpt pin (5 V, regulated to 3.9 V using resistors on this circuit)
  pinMode(laserPin, OUTPUT);

  lastTime = millis();

//  Serial.begin(9600);
}

void loop() 
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  const int buttonRepeatDelayMs = 500;
  if (buttonState != lastButtonState && millis() > lastTime + buttonRepeatDelayMs)
  {
    lastTime = millis();
    
    // Flip LED/LASER state
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

    // Set inbuilt LED to be on or off. Used for testing only when laser isn't present.
//    digitalWrite(ledPin, ledState);

    // Set laser to be on or off
    digitalWrite(laserPin, ledState);
  }

  lastButtonState = buttonState;

  // Read potentiometer from analog input
  potLevel = analogRead(A0); // reads value between 0 and 1023 (10 bit input)
  // Send potentiometer dial position to the servo
  analogWrite(servoPwmPin, potLevel >> 5);
//  Serial.println(potLevel >> 2, DEC); // Leave this commented out when doing actual exposure, as the LED light could interfere with the holographic plate

  // Limit loop refresh rate to 20 Hz
  delay(50);
}
