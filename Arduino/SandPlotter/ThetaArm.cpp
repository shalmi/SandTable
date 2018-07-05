#include "Arduino.h"
#include "ThetaArm.h"
#include "Stepper.h"

ThetaArm::ThetaArm(int directionPin, int enablePin, int stepPin, int _hallPin)
{ //For use with normal stepper
    stepper.Init(directionPin, enablePin, stepPin);
    hallPin = _hallPin;
    armState = 0;
}
/// <summary> .... </summary>
/// <param name="bar"> .... </param>
/// <returns> .... </returns>
void RadiusArm::Setup()
{
    // Is there any setup to do
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

bool RadiusArm::Calibrate_Theta_Axis()
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
void RadiusArm::EnableMotor(){
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