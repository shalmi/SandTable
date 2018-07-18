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
void ThetaArm::Setup()
{
    // Is there any setup to do
}

void ThetaArm::TakeStep(){
    
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

bool ThetaArm::ReverseDirectionOnBump() //rewrite to work with hall effect sensor
{
    bool didBump = false;
    if ((digitalRead(hallPin) == HIT) && (currentDirection == counterClockWise))
    {
        currentDirection = clockWise;
        didBump = true;
    }
    else if ((digitalRead(hallPin) == HIT) && (currentDirection == clockWise))
    {
        currentDirection = counterClockWise;
        didBump = true;
    }
    // digitalWrite(yDirPin, currentDirection); // Enables the motor to move in a particular direction
    stepper.ChangeDirection(currentDirection); // Enables the motor to move in a particular direction
    return didBump;
}

bool ThetaArm::Calibrate_Theta_Axis()
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
bool ThetaArm::Startup()
{
    TakeStep();
    if (ReverseDirectionOnBump())
    {
        return true;
    }
    return false;
}
void ThetaArm::DisableMotor(){
    stepper.DisableMotor();
}
void ThetaArm::EnableMotor(){
    stepper.EnableMotor();
}
void ThetaArm::SetDestination(long destination){
    desiredLocation = destination;
    armState = GO_TO_POINT;
    stepper.EnableMotor();
}

void ThetaArm::ChangeDirection(bool desiredDirection){
    if (currentDirection != desiredDirection){
        stepper.ChangeDirection(desiredDirection);
        currentDirection = desiredDirection;
    }
}
bool ThetaArm::MoveTowardsDestination(){ //This is not a good function and needs to fix issues with the fact that 361 = 1 etc
    if(currentLocation<desiredLocation){
        ChangeDirection(counterClockWise);
        TakeStep(); // Move Towards Idler
    }
    else if (currentLocation>desiredLocation){
        ChangeDirection(clockWise);
        TakeStep(); // Move Towards Motor
    }
    else{
        return true;
    }
    return false;
}

void ThetaArm::ArmLoop(){

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