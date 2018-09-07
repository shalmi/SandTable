#include "Arduino.h"
#include "ThetaArm.h"
#include "Stepper.h"


ThetaArm::ThetaArm(int directionPin, int enablePin, int stepPin, int _hallPin)
{ //For use with normal stepper
    stepper.Init(directionPin, enablePin, stepPin, 10);
    hallPin = _hallPin;
    armState = 0;
}
/// <summary> .... </summary>
/// <param name="bar"> .... </param>
/// <returns> .... </returns>
void ThetaArm::Setup()
{
    pinMode(hallPin, INPUT);
    // Is there any setup to do

}

void ThetaArm::TakeStep(){
    if(stepper.OneStepIfTime())
    {   
        if (currentDirection) { //if CCW
            currentLocation+=1;
        }
        else{ // CW
            currentLocation-=1;
        }
    }
}
bool ThetaArm::OnHallEffect()
{
    if (analogRead(hallPin) <= magnetSensedValue)
    {
        return true;
    }
    return false;
}

// find correct sensor value for hall effect and then return true when at a sensor.
// also sets the 0 location!
bool ThetaArm::CalibrateHallEffectSensor() //rewrite to work with hall effect sensor
{
    int testSamples = 750;
    if(hallEffectCalibrationCounter<testSamples)
    {
        int currentRead = analogRead(hallPin);
        if ( (currentRead <= 400) && (currentRead != 0))
        {
            hallEffectCalibrationCounter++;
            if(currentRead < magnetSensedValue)
            {
                magnetSensedValue = currentRead;
                // currentLocation=0;
            }
        }
        TakeStep();
    }//endif
    else if(hallEffectCalibrationCounter == testSamples)
    {
        Serial.print("HallEffect Calibration at: ");
        Serial.println(magnetSensedValue);
        hallEffectCalibrationCounter++;
        magnetSensedValue = magnetSensedValue*1.05;
    }
    else{
        TakeStep();
        int currentRead = analogRead(hallPin);
        if (currentRead <= magnetSensedValue)
        {
            ChangeDirection(clockWise);
            currentLocation=0;
            return true;
        }
    }


    return false;
}

// make a 360 and measure the steps 
bool ThetaArm::Calibrate_Theta_Axis()
{
    currentLocation = 0;
    return true; //nothing can exist here till hall effect sensor exists
    /// DUMMY till this is removed. this func was causing too many issues.
    // and not really working too great either



    // currentLocation = 0;
    TakeStep(); // this needs to return a bool...also wtf
    fullDistance++;
    if ( (abs(currentLocation)>300) && OnHallEffect())
    {
        DisableMotor();
        Serial.print("Full Distance of Steps for Theta: ");
        Serial.println(fullDistance);
        currentLocation = 0;
        return true; //nothing can exist here till hall effect sensor exists
    }
    return false;
}
bool ThetaArm::Startup()
{
    if(startupFinished){
        return true;
    }
    else
    {
        stepper.Startup();
        if (CalibrateHallEffectSensor())
        {
            startupFinished = true;
            stepsInRotationOverTwoPi = (float)TheoreticalStepsInRotation/dosPI;
            Serial.println("ThetaArm Startup Finished");
            return true;
        }
        return false;
    }
}
void ThetaArm::DisableMotor(){
    // stepper.DisableMotor();
}
void ThetaArm::EnableMotor(){
    stepper.EnableMotor();
}
void ThetaArm::SetDestination(long destination){
    // TODO: michael fix this line below. make all the floats floaty
    desiredLocation = (destination*8200)/360;
    armState = GO_TO_POINT;
    stepper.EnableMotor();
}

void ThetaArm::SetDestinationAsCalculatedRadians(float nextMajorTheta){
    // nextMajorTheta should be theta in radians....
    // ....so 0-2π (6.28319 or so)
    //we need to scale this to our totalDistance
    desiredLocation = nextMajorTheta * (float)stepsInRotationOverTwoPi; //*8200)/360;
    // Serial.println(desiredLocation);
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
        TakeStep();
    }
    else if (currentLocation>desiredLocation){
        ChangeDirection(clockWise);
        TakeStep(); 
    }
    else{
        return true;
    }
    return false;
}

byte ThetaArm::ArmLoop(){

    switch (armState)
    {
        case IDLE:
            return 1;
            break;
        case GO_TO_POINT:
            if(MoveTowardsDestination()){
                DisableMotor();
                armState = IDLE;
                return 1;
            }
            break;
    
        default:
            break;
    }
    return 0;
}