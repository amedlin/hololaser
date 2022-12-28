#include <Servo.h>

/* Control software for Arduino designed for holographic plate exposure control.
 *
 * Behaviour:
 * Note: in any state other than idle state (SYSTEM_IDLE), a long button press turns off the laser and closes the aperture. (See longButtonPressDurationMilliSeconds.)
 * - Starts in idle state (SYSTEM_IDLE: laser off, awaiting first button press). While idle, servo calibration can be performed to ensure laser aperture is properly blocked (while the laser is off).
 * - Upon first button press, enters laser warmup state (LASER_WARMUP: aperture closed). Stays in this state for a minimum of laserWarmUpTimeMilliSeconds.
 *   Additional button presses prior to this are ignored. Inbuilt LED flashes to indicate laser warmup in progress.
 *   After warmup complete, inbuilt LED flashing stops and system automatically progresses to laser ready state (LASER_READY).
 * - In laser ready state (LASER_READY), awaits next button press to initiate an exposure.
 * - When button pressed (not held) in laser ready state (LASER_READY), initiates an exposure. This starts by entering APERTURE_OPENING state until the aperture is open,
 *   then automatically progresses to LASER_EXPOSURE state.
 * - In LASER_EXPOSURE state, exposure will end upon a single button press, or when the exposure times out. (See maxExposureTimeMilliSeconds.)
 * - When exposure ends, laser is immediately turned off (LASER_OFF state), then immediately progresses to APERTURE_CLOSING state.
 * - In APERTURE_CLOSING state, the aperture is returned to the closed position (a time allowance is made, since we can't read the servo position).
 *   Once the time-allowance for aperture closure has passed, the laser automatically returns to the LASER_WARMUP state in preparation for the next exposure.
 * - When you wish to turn the system off (i.e, return to SYSTEM_IDLE state), press and long-hold the button to turn off laser and return to idle. (See longButtonPressDurationMilliSeconds.)
 */

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
const int laserPin = 4;
const uint8_t potPin = A0;
const int servoPwmPin = 11;
const int servoNeutralAngle = 90;
const int servoOpenAngle = 90 + 60;
const int potNoiseVariance = 2;
const int laserWarmUpTimeMilliSeconds = 5 * 60 * 1000; // 5 minutes
const int longButtonPressDurationMilliSeconds = 1000; // 1 sec button hold is interpreted as a long button press
const int maxExposureTimeMilliSeconds = 30 * 1000;

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

enum class SystemState
{
  SYSTEM_IDLE = 0,
  LASER_WARMUP,
  LASER_READY,
  APERTURE_OPENING,
  LASER_EXPOSURE,
  LASER_OFF,
  APERTURE_CLOSING
};

SystemState systemState = SystemState::SYSTEM_IDLE;

void updateSystemIdle(unsigned long timeMillis);
void updateLaserWarmUp(unsigned long timeMillis);
void updateLaserReady(unsigned long timeMillis);
void updateApertureOpening(unsigned long timeMillis);
void updateLaserExposure(unsigned long timeMillis);
void updateLaserOff(unsigned long timeMillis);
void updateAperturnClosing(unsigned long timeMillis);

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
  unsigned long thisTimeMillis = millis();

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  bool doServoUpdate = false;

  const int buttonRepeatDelayMs = 500;
  if (buttonState != lastButtonState && thisTimeMillis > lastTime + buttonRepeatDelayMs)
  {
    lastTime = thisTimeMillis;
    
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

void updateSystemIdle(unsigned long timeMillis)
{

}

void updateLaserWarmUp(unsigned long timeMillis)
{

}

void updateLaserReady(unsigned long timeMillis)
{

}

void updateApertureOpening(unsigned long timeMillis)
{

}

void updateLaserExposure(unsigned long timeMillis)
{

}

void updateLaserOff(unsigned long timeMillis)
{

}

void updateAperturnClosing(unsigned long timeMillis)
{

}
