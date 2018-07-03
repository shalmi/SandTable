#include "Arduino.h"
#include "Stepper.h"

// const uint8_t fan_pin = 2;
// int directionPin;
// int enablePin;
// int stepPin;
// int speed = 1;
Stepper::Stepper(int _directionPin,int _enablePin,int _stepPin)
: directionPin(_directionPin), enablePin(_enablePin), stepPin(_stepPin)
{
  speed = 1;
}

void Stepper::Setup(){
  // Sets the three pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(directionPin,OUTPUT);
  pinMode(enablePin,OUTPUT);
  // Set Enable low ...so the Stepper Works 
  digitalWrite(enablePin,LOW); 
}

void Stepper::OneStep(){
    int stepperSpeed = speed*50;
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(stepperSpeed); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(stepperSpeed); 
}

void Stepper::SetSpeed(int _speed){
  speed = _speed;
}

void Stepper::ChangeDirection(bool newDirection){
  digitalWrite(directionPin,newDirection);
}


