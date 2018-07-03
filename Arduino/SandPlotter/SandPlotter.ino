/*     SandPlotter
 *      
 *  by Michael Yeroshalmi
 *  
 */

#include "Arduino.h"
#include "Stepper.h"
#include "SimpleTimer.h"
#include "Definitions.h"

//Add 54 to num
// static const uint8_t A0 = 54;

// defines pins numbers
//const int pushButton = 14;
const int innerLimit = 14;
const int outerLimit = 15;
const int pushButton = 14;
const int yEnable = 56;  //A2
const int yStepPin = 60; //A6
const int yDirPin = 61;  //A7
// const int stepsForRevolution = 3200;
const bool outward = true;
const bool inward = false;
bool currentDirection = outward;
const bool HIT = true;
const bool NOT_HIT = false;

int state = STARTUP;
long stepCounter = 0;

SimpleTimer timer;

Stepper r_stepper = Stepper(yDirPin, yEnable, yStepPin);
// Stepper theta_stepper = Stepper(yDirPin,yEnable,yStepPin);

void setup()
{
    Serial.begin(9600);
    timer.setInterval(15000, RepeatTask);
    //  pinMode(pushButton, INPUT_PULLUP);
    pinMode(innerLimit, INPUT_PULLUP);
    pinMode(outerLimit, INPUT_PULLUP);
    r_stepper.Setup();
}

// function to be called repeatedly
void RepeatTask()
{
    Serial.println("15 second timer");
}

bool ReverseDirectionOnBump()
{
    bool didBump = false;
    if ((digitalRead(outerLimit) == HIT) && (currentDirection == outward))
    {
        currentDirection = inward;
        didBump = true;
    }
    else if ((digitalRead(innerLimit) == HIT) && (currentDirection == inward))
    {
        currentDirection = outward;
        didBump = true;
    }
    digitalWrite(yDirPin, currentDirection); // Enables the motor to move in a particular direction
    return didBump;
}
// void InitialCalibration(){

// }

void Calibrate_R_Axis(){
    r_stepper.OneStep();
    stepCounter += 1;
    if(ReverseDirectionOnBump()){
        long lastStepCounter = stepCounter; //TODO: Declare this somewhere or keep it idno
        Serial.println("Steps between switches = "+ String(stepCounter));
        stepCounter = 0;
    }
}

void loop()
{
    switch (state)
    {
    case STARTUP:
        r_stepper.OneStep();
        if(ReverseDirectionOnBump()) { 
            state = CALIBRATION;
            Serial.println("State Entering `CALIBRATION` Mode...");
            }
        break;

    case CALIBRATION:
        Calibrate_R_Axis();
        
    default:
        state = 0;
        Serial.println("Default Catch `state` var in unkown State: "+String(state));
        while(true){} //stop everything here.

    }//switch on state
    
}//loop
