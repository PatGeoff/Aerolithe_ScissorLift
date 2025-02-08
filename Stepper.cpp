// stepper.cpp

// FOR SCISSOR LIFT
// If the lift reaches the bottomSwitch, then goDown will be false until position reaches a value of
// If the lift reaches the topSwitch, then goUp will be set to false until position reaches a value of


#include "stepper.h"
#include <WiFiUDP.h>
#include "aerolitheUdp.h"

const int stepPin = 12;       // Pin connected to the STEP pin of the stepper driver
const int dirPin = 13;        // Pin connected to the DIR pin of the stepper driver
const int topLimitPin = 27;   // Pin for the limit switch when the lift moves up
const int bottomLimitPin = 25;  // Pin for the limit switch when the lift moves down
int enablePin = 27; 

const int microsteps = 16;  // microsteps à 1/16
const int acceleration = 1000;

const long maxPosition = 61200;  // Maximum allowable position
const int maxSpeed = 20000;       // Maximum speed variable

bool runSpeedBool = false;

bool allowMoveForward = true;
bool allowMoveBackward = true;

// An instance of AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// Define variables for the limit switch flags
volatile bool upperLimitInterruptTriggered = false;
volatile bool lowerLimitInterruptTriggered = false;

// Interrupt service routines
// NOT A GOOD IDEA to put processor heavy fuctions in the IRAM_ATTR(), keep it at minimum
void IRAM_ATTR onFarLimit() {
  upperLimitInterruptTriggered = true;
  allowMoveBackward = false;
  runSpeedBool = true;
  stepper.setSpeed(0);
}

void IRAM_ATTR onNearLimit() {
  lowerLimitInterruptTriggered = true;
  allowMoveForward = false;
  runSpeedBool = true;
  stepper.setSpeed(0);
}

void initializeLimitSwitches() {
  // Set up the pins
  pinMode(topLimitPin, INPUT_PULLUP);
  pinMode(bottomLimitPin, INPUT_PULLUP);
  pinMode(enablePin, OUTPUT);
  // Attach interrupts to the limit switch pins
  //attachInterrupt(digitalPinToInterrupt(topLimitPin), onFarLimit, FALLING);
  //attachInterrupt(digitalPinToInterrupt(bottomLimitPin), onNearLimit, FALLING);
  digitalWrite(enablePin, HIGH);  // Disable the stepper driver after the move  
  Serial.print("Limit switches initiazized");

}




void performStepperMotorMoveTo(int speed, long position) {
  digitalWrite(enablePin, LOW);  // Enable the stepper driver
  runSpeedBool = false;
  if ((position > stepper.currentPosition() && allowMoveForward) || (position < stepper.currentPosition() && allowMoveBackward)) {
    if (position <= maxPosition) {
      stepper.setMaxSpeed(speed);
      stepper.setAcceleration(acceleration);
      stepper.moveTo(position);
    } else {
      Serial.println("Stepper -> Requested position exceeds maximum allowable position.");
    }
  } else {
    Serial.println("Stepper -> Movement not allowed in the requested direction.");
  }

  // Call run() method to make the stepper move asynchronously
  long delta = abs(position - stepper.currentPosition());
  static long stepsToRun = 0;

  if (stepsToRun == 0) {
    stepsToRun = delta;
  }

  if (stepsToRun > 0) {
    stepper.run();
    stepsToRun--;
  }
  digitalWrite(enablePin, HIGH);  // Disable the stepper driver after the move
}

void performStepperMotorRunSpeed(int speed) {
  digitalWrite(enablePin, LOW);  // Enable the stepper driver after the move
  runSpeedBool = true;
  Serial.println("Stepper -> Run Speed Start");
  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(acceleration);
  stepper.setSpeed(speed);
  if (speed == 0){
    stepper.stop();
    digitalWrite(enablePin, HIGH);  // Disable the stepper driver after the move  
  }
}

