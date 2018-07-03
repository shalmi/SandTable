#include "Arduino.h"
#include "RadiusArm.h"
#include "Stepper.h"

const bool HIT = true;
const bool NOT_HIT = false;
const bool outward = true;
const bool inward = false;
long stepCounter = 0;
bool currentDirection = outward;

RadiusArm::RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop)
{
    stepper.Init(directionPin, enablePin, stepPin);
    innerEndStop = _innerEndStop;
    outerEndStop = _outerEndStop;
}

void RadiusArm::Setup()
{
    //set Limit Switches as Inputs with Pullup Resistors
    pinMode(innerEndStop, INPUT_PULLUP);
    pinMode(outerEndStop, INPUT_PULLUP);
}

void RadiusArm::ChangeDirection(bool newDirection)
{
    stepper.ChangeDirection(newDirection);
}

bool RadiusArm::ReverseDirectionOnBump()
{
    bool didBump = false;
    if ((digitalRead(outerEndStop) == HIT) && (currentDirection == outward))
    {
        currentDirection = inward;
        didBump = true;
    }
    else if ((digitalRead(innerEndStop) == HIT) && (currentDirection == inward))
    {
        currentDirection = outward;
        didBump = true;
    }
    // digitalWrite(yDirPin, currentDirection); // Enables the motor to move in a particular direction
    stepper.ChangeDirection(currentDirection); // Enables the motor to move in a particular direction
    return didBump;
}

void RadiusArm::Calibrate_R_Axis()
{
    stepper.OneStep();
    stepCounter += 1;
    if (ReverseDirectionOnBump())
    {
        long lastStepCounter = stepCounter; //TODO: Declare this somewhere or keep it idno
        Serial.println("Steps between switches = " + String(stepCounter));
        stepCounter = 0;
    }
}
bool RadiusArm::Startup()
{
    stepper.OneStep();
    if (ReverseDirectionOnBump())
    {
        return true;
    }
    return false;
}
