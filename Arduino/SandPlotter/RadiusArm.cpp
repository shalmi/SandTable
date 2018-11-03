#include "Arduino.h"
#include "RadiusArm.h"
#include "Stepper.h"


//It takes roughly 80.36 steps to move 1mm
// Center is 6000
//Right Before End Stop is 16750

RadiusArm::RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop)
{ //For use with normal stepper
    stepper.Init(directionPin, enablePin, stepPin, 1); //was 1
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
        // Serial.println("There is a wall there :P Stop trying to go there!");
        return false;
    }
    if ((digitalRead(innerEndStop) == HIT) && (currentDirection == inward)){
        // Serial.println("There is a wall there :P Stop trying to go there!");
        return false;
    }
    return true;
}

bool RadiusArm::TakeStep(){
    
    if (RoomToMove()) {
        if(stepper.OneStepIfTime())
        {
            if (currentDirection) { //if outward
                currentLocation+=1;
            }
            else{
                currentLocation-=1;
            }
            return true;
        }
    }
    return false;
}

bool RadiusArm::Calibrate_R_Axis()
{
    if(calibrationFinished)
    {
        return true;
    }
    else
    {
        if ( TakeStep() )
        {
            stepCounter += 1;
        }
        
        if (ReverseDirectionOnBump())
        {
            fullDistance = stepCounter;
            Serial.println("Steps between switches = " + String(stepCounter));
            stepCounter = 0;

            // this line was currentLocation = 0;
            // instead of setting current location to 0
            // this will set it to ~ -6800 which should make it move to center at start
            currentLocation = -1*centerOffset;

            DisableMotor();
            
            // fullDistance should be massaged
            // aka delete the distance to center ~6800
            // also delete 200 or so for the switch
            // this should be done once when full distance is calibrated
            
            // lets assume this leaves fullDistance to be about 8000
            // calculate this line ONLY ONCE since it will be use Constantly
            fullDistOverRange = fullDistance/(float)500;

            calibrationFinished = true;
            return true;
        }
        return false;
    }
}
bool RadiusArm::Startup()
{
    if(startupFinished){
        return true;
    }
    else
    {
        stepper.Startup();
        TakeStep();
        if (ReverseDirectionOnBump())
        {
            Serial.println("RadiusArm Startup Finished");
            startupFinished = true;
            return true;
        }
        return false;
    }
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
void RadiusArm::SetSpeed(int speed){
    stepper.SetSpeed(speed);
}
void RadiusArm::SetDestinationAsCalculatedR(float destination)
{
    // destination given should already be massaged
    // Because r shouldnt ever be negative this should just be 0-500
    // is 0-500 enough...should be since it is going to be a float

    destination = destination*fullDistOverRange;
    // Serial.println(destination);
    SetDestination( (long)destination );
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

byte RadiusArm::ArmLoop(){

    switch (armState)
    {
        case IDLE:
            return 1;
            break;
        case GO_TO_POINT:
            if(MoveTowardsDestination()){
                stepper.DisableMotor();
                armState = IDLE;
                return 1;
            }
            break;
    
        default:
            break;
    }
    return 0;
}