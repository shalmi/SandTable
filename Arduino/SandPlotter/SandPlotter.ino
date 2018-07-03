/*     SandPlotter
 *      
 *  by Michael Yeroshalmi
 *  
 */

#include "Arduino.h"
#include "Stepper.h"
#include "SimpleTimer.h"
#include "Definitions.h"
#include "RadiusArm.h"

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

int state = STARTUP;

SimpleTimer timer;

RadiusArm radiusArm = RadiusArm(yDirPin, yEnable, yStepPin, innerLimit, outerLimit);
// Stepper theta_stepper = Stepper(yDirPin,yEnable,yStepPin);

void setup()
{
    Serial.begin(9600);
    Serial.println("Michaels Sand Table Starting Up");
    timer.setInterval(15000, RepeatTask);
    //  pinMode(pushButton, INPUT_PULLUP);
    radiusArm.Setup();
}

// function to be called repeatedly
void RepeatTask()
{
    Serial.println("15 second timer");
}

// void InitialCalibration(){

// }

void loop()
{
    switch (state)
    {
    case STARTUP:
        if (radiusArm.Startup())
        {                        //radiusArm has hit a limitSwitch
            state = CALIBRATION; //move on to calibration
            Serial.println("State Entering `CALIBRATION` Mode...");
        }
        break;

    case CALIBRATION:
        radiusArm.Calibrate_R_Axis();
        break;

    default:
        state = 0;
        Serial.println("Default Catch `state` var in unkown State: " + String(state));
        while (true)
        {
        } //stop everything here.

    } //switch on state

} //loop
