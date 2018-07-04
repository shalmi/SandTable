#include "Arduino.h"
#include "Stepper.h"
#include <TMC2130Stepper.h>

// const uint8_t fan_pin = 2;
// int directionPin;
// int enablePin;
// int stepPin;
// int speed = 1;
Stepper::Stepper()
{
    speed = 1;
}

void Stepper::Init(int _directionPin, int _enablePin, int _stepPin)
{
    // Save the pins
    directionPin = _directionPin;
    enablePin = _enablePin;
    stepPin = _stepPin;

    // Sets the three pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    // Set Enable low ...so the Stepper Works
    EnableMotor();
}

//IT DOESNT WORK>>>DONT USE
void Stepper::Init(int _directionPin, int _enablePin, int _stepPin, int _chipSelectPin)
{
    // Save the pins
    directionPin = _directionPin;
    enablePin = _enablePin;
    stepPin = _stepPin;

    //THIS INIT IS FOR THE TMC2130
    //IT DOESNT WORK>>>DONT USE
    TMC2130Stepper TMC2130 = TMC2130Stepper(enablePin, directionPin, stepPin, _chipSelectPin);
    TMC2130.begin(); // Initiate pins and registeries
    TMC2130.SilentStepStick2130(600); // Set stepper current to 600mA
    TMC2130.stealthChop(1); // Enable extremely quiet stepping
    TMC2130.shaft_dir(0);

    // Sets the three pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    // Set Enable low ...so the Stepper Works
    EnableMotor();
}

void Stepper::OneStep()
{
    int stepperSpeed = speed * 50;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperSpeed);
}

void Stepper::SetSpeed(int _speed)
{
    speed = _speed;
}

void Stepper::ChangeDirection(bool newDirection)
{
    digitalWrite(directionPin, newDirection);
}
void Stepper::DisableMotor(){
    digitalWrite(enablePin,HIGH);
}

void Stepper::EnableMotor(){
    digitalWrite(enablePin,LOW);
}