#include "Arduino.h"
#include "Stepper.h"

const uint8_t fan_pin = 2;

Stepper::Stepper(int directionPin,int enablePin,int stepPin){
  //set the pins variables here
  
}

void Stepper::Setup(){
    // Sets the three pins as Outputs
  pinMode(yStepPin,OUTPUT); //FixMe: these variable name
  pinMode(yDirPin,OUTPUT);
  pinMode(yEnable,OUTPUT);
}

void Stepper::SetSpeed(int speed){
  //set speed here
}

int Stepper::rotation(){
  int returnval = 12;
  return (returnval);
}


