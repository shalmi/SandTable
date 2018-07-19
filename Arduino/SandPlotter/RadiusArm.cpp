#include "Arduino.h"
#include "RadiusArm.h"
#include "Stepper.h"


//It takes roughly 80.36 steps to move 1mm

RadiusArm::RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop)
{ //For use with normal stepper
    stepper.Init(directionPin, enablePin, stepPin);
    innerEndStop = _innerEndStop;
    outerEndStop = _outerEndStop;
    armState = 0;
}
RadiusArm::RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop, int _chipSelectPin)
{ // For use with TMC2130
    stepper.Init(directionPin, enablePin, stepPin, _chipSelectPin);
    innerEndStop = _innerEndStop;
    outerEndStop = _outerEndStop;
    armState = 0;
}

void RadiusArm::Setup()
{
    //set Limit Switches as Inputs with Pullup Resistors
    pinMode(innerEndStop, INPUT_PULLUP);
    pinMode(outerEndStop, INPUT_PULLUP);
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

bool RadiusArm::RoomToMove(){
    if ((digitalRead(outerEndStop) == HIT) && (currentDirection == outward)){
        Serial.println("There is a wall there :P Stop trying to go there!");
        return false;
    }
    if ((digitalRead(innerEndStop) == HIT) && (currentDirection == inward)){
        Serial.println("There is a wall there :P Stop trying to go there!");
        return false;
    }
    return true;
}

void RadiusArm::TakeStep(){
    
    if (RoomToMove()) {
        stepper.OneStep();
        
        if (currentDirection) { //if outward
            currentLocation+=1;
        }
        else{
            currentLocation-=1;
        }
        
    }
}

bool RadiusArm::Calibrate_R_Axis()
{
    TakeStep();
    stepCounter += 1;
    if (ReverseDirectionOnBump())
    {
        fullDistance = stepCounter;
        Serial.println("Steps between switches = " + String(stepCounter));
        stepCounter = 0;
        currentLocation = 0;
        return true;
    }
    return false;
}
bool RadiusArm::Startup()
{
    TakeStep();
    if (ReverseDirectionOnBump())
    {
        return true;
    }
    return false;
}
void RadiusArm::DisableMotor(){
    stepper.DisableMotor();
}
void RadiusArm::Live(){
    stepper.EnableMotor();
}
void RadiusArm::SetDestination(long destination){
    desiredLocation = destination;
    armState = GO_TO_POINT;
    stepper.EnableMotor();
}

void RadiusArm::ChangeDirection(bool desiredDirection){
    if (currentDirection != desiredDirection){
        stepper.ChangeDirection(desiredDirection);
        currentDirection = desiredDirection;
    }
}
bool RadiusArm::MoveTowardsDestination(){
    if(currentLocation<desiredLocation){
        ChangeDirection(outward);
        TakeStep(); // Move Towards Idler
    }
    else if (currentLocation>desiredLocation){
        ChangeDirection(inward);
        TakeStep(); // Move Towards Motor
    }
    else{
        return true;
    }
    return false;
}

void RadiusArm::ArmLoop(){

    switch (armState)
    {
        case IDLE:
            break;
        case GO_TO_POINT:
            if(MoveTowardsDestination()){
                stepper.DisableMotor();
                armState = IDLE;
            }
            break;
    
        default:
            break;
    }
}