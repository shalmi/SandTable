#include "Arduino.h"
#include "Stepper.h"

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