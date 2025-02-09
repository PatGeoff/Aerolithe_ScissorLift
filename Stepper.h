//Stepper.h

#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>

// External declarations
extern AccelStepper stepper;

extern const int topLimitPin;
extern const int bottomLimitPin;

extern volatile bool lowerLimitInterruptTriggered;
extern volatile bool upperLimitInterruptTriggered;

extern const int microsteps;
extern bool runSpeedBool;
extern bool allowMoveForward;
extern bool allowMoveBackward;
extern int enablePin;
extern const int stepPin;       // Pin connected to the STEP pin of the stepper driver
extern const int dirPin; 

// Function declarations
void initializeLimitSwitches();
void performStepperMotorMoveTo(int speed, long position);
void performStepperMotorRunSpeed(int speed);

#endif  // STEPPER_H
