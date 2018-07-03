#include "Arduino.h"
#include "RadiusArm.h"
#include "Stepper.h"

Stepper stepper = Stepper(1, 2, 3);

RadiusArm::RadiusArm(int directionPin,int enablePin,int stepPin)
{
        // Stepper stepper = Stepper(directionPin, enablePin, stepPin);

}

void RadiusArm::Setup(){
  // Sets the three pins as Outputs
    stepper.Setup();
}

void RadiusArm::OneStep(){
    stepper.OneStep();
}

void RadiusArm::Setspeed(int _speed){
  stepper.SetSpeed(_speed);
}

void RadiusArm::ChangeDirection(bool newDirection){
  stepper.ChangeDirection(newDirection);
}


