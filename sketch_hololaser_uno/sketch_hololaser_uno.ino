#include <Servo.h>

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
const int laserPin = 4;
const uint8_t potPin = A0;
const int servoPwmPin = 11;
const int servoNeutralAngle = 90;
const int servoOpenAngle = 90 + 60;
const int potNoiseVariance = 2;

// Input variables
int buttonState = 0;  // variable for reading the pushbutton status
int potLevel; // potentiometer position, used to calibrate the servo motor angle
int initialPotLevel;

// Global state
int lastButtonState = buttonState;
int ledState = LOW;
Servo servo;
int servoTargetAngle = servoNeutralAngle;
int potBias = 0;

unsigned long lastTime;

// Debugging vars
const bool mirrorInbuiltLed = false;
const bool servoTestMode = false;

// enum SystemState
// {
//   UNDEFINED = 0,
//   WAITING_INPUT,
//   INITIALIZING,
//   LASER_WARMUP,
//   APERTURE_OPENING,
//   LASER_EXPOSURE,
//   LASER_OFF,
//   APERTURE_CLOSING
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

  servo.attach(servoPwmPin);
  if (!servoTestMode)
  {
    servo.write(servoNeutralAngle);
  }

  // Initialize reference pot level (to whatever position it starts at)
  potLevel = initialPotLevel = analogRead(potPin);
}

void loop() 
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  bool doServoUpdate = false;

  const int buttonRepeatDelayMs = 500;
  if (buttonState != lastButtonState && millis() > lastTime + buttonRepeatDelayMs)
  {
    lastTime = millis();
    
    // Flip LED/LASER state
    if (ledState == HIGH) 
    {
      // turn LED off:
      ledState = LOW;
      servoTargetAngle = servoNeutralAngle;
    }
    else
    {
      // turn LED on:
      ledState = HIGH;
      servoTargetAngle = servoOpenAngle;
    }
    doServoUpdate = true;

    // Set inbuilt LED to be on or off. Used for testing only when laser isn't present.
    if (mirrorInbuiltLed)
    {
      digitalWrite(ledPin, ledState);
    }

    // Set laser to be on or off
    digitalWrite(laserPin, ledState);
  }

  lastButtonState = buttonState;

  if (servoTestMode)
  {
    // Read potentiometer from analog input
    potLevel = analogRead(potPin); // reads value between 0 and 1023 (10 bit input)
    // Send potentiometer dial position to the servo
    analogWrite(servoPwmPin, potLevel >> 2);
    // Serial.println(potLevel >> 2, DEC); // Leave this commented out when doing actual exposure, as the LED light could interfere with the holographic plate
    doServoUpdate = false;
  }
  else
  {
    // check for significant change in pot position
    int currentPotSample = analogRead(potPin);
    if (abs(currentPotSample - potLevel) > potNoiseVariance)
    {
      potLevel = currentPotSample;
      potBias = (potLevel - initialPotLevel) >> 4;
      doServoUpdate = true;
    }
  }

  if (doServoUpdate)
  {
      servo.write(servoTargetAngle + potBias);
  }

  // Limit loop refresh rate to 20 Hz
  delay(50);
}
