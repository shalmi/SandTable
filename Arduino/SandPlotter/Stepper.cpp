#include "Arduino.h"
#include "Stepper.h"
// #include <TMC2130Stepper.h>

// const uint8_t fan_pin = 2;
// int directionPin;
// int enablePin;
// int stepPin;
// int speed = 1;
Stepper::Stepper()
{
    // speed = 1;
}

void Stepper::Init(int _directionPin, int _enablePin, int _stepPin, int _speed)
{
    // Save the pins
    directionPin = _directionPin;
    enablePin = _enablePin;
    stepPin = _stepPin;
    speed = _speed;
    // Sets the three pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    // Set Enable low ...so the Stepper Works
    EnableMotor();
    timeAtNextPulse = micros() + (speed*50);
}

//IT DOESNT WORK>>>DONT USE
// void Stepper::Init(int _directionPin, int _enablePin, int _stepPin, int _chipSelectPin)
// {
//     // Save the pins
//     directionPin = _directionPin;
//     enablePin = _enablePin;
//     stepPin = _stepPin;

//     //THIS INIT IS FOR THE TMC2130
//     //IT DOESNT WORK>>>DONT USE
//     TMC2130Stepper TMC2130 = TMC2130Stepper(enablePin, directionPin, stepPin, _chipSelectPin);
//     TMC2130.begin(); // Initiate pins and registeries
//     TMC2130.SilentStepStick2130(600); // Set stepper current to 600mA
//     TMC2130.stealthChop(1); // Enable extremely quiet stepping
//     TMC2130.shaft_dir(0);

//     // Sets the three pins as Outputs
//     pinMode(stepPin, OUTPUT);
//     pinMode(directionPin, OUTPUT);
//     pinMode(enablePin, OUTPUT);
//     // Set Enable low ...so the Stepper Works
//     EnableMotor();
// }

// takes a half step if it is time to half step
// returns true if a step was taken AND finished, false otherwise
bool Stepper::OneStepIfTime()
{
    if(timeAtNextPulse<1000){ //just to deal with not missing steps and rollover for micros()
        delayMicroseconds(1000); //should only happen every 70 hours
    }
    if(micros()>timeAtNextPulse)//if( its time to take a step)
    {
        if(currentlyMidStep)//if(we are in the middle of a step)
        {
            digitalWrite(stepPin,LOW);//finish step
            timeAtNextPulse = micros() + (speed*50);//reset timer
            currentlyMidStep = false;
            return true; //a step was taken and FINISHED
        }
        else{
            digitalWrite(stepPin, HIGH);//take first half of step
            currentlyMidStep = true;
            timeAtNextPulse = micros() + (speed*50);//reset timer
        }
    }
    return false; //it's not time for a step you fool (maybe half step)

}
//deprecated to OneStepIfTime
// void Stepper::OneStep()
// {
//     int stepperSpeed = speed * 50;
//     digitalWrite(stepPin, HIGH);
//     delayMicroseconds(stepperSpeed);
//     digitalWrite(stepPin, LOW);
//     delayMicroseconds(stepperSpeed);
// }

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