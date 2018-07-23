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
#include "ThetaArm.h"

//Add 54 to num
// static const uint8_t A0 = 54;

// defines pins numbers
const int innerLimit = 14;
const int outerLimit = 15;
const int pushButton = 14;
//RadiusMotorPINS
const int rEnable = 56;  //A2
const int rStepPin = 60; //A6
const int rDirPin = 61;  //A7
const int rCsPin = 62; //A8
//ThetaMotorPINS
const int thetaDirPin = 63;  //A9
const int thetaStepPin = 64; //A10
const int thetaEnable = 65;  //A11
const int thetaHallPin = 66; //THIS IS NOT TRUE....shhhh
// const int thetaCsPin = 62; //A8
// const int stepsForRevolution = 3200;

int state = 0;

SimpleTimer timer;

RadiusArm radiusArm = RadiusArm(rDirPin, rEnable, rStepPin, innerLimit, outerLimit); //Works for Normal Driver
ThetaArm thetaArm = ThetaArm(thetaDirPin,thetaEnable,thetaStepPin,thetaHallPin);
// RadiusArm radiusArm = RadiusArm(rDirPin, rEnable, rStepPin, innerLimit, outerLimit, rCsPin); //For TMC2130

// Stepper theta_stepper = Stepper(rDirPin,rEnable,rStepPin);

// #include <TMC2130Stepper.h>
// TMC2130Stepper TMC2130 = TMC2130Stepper(rEnable, rDirPin, rStepPin, rCsPin);

void setup()
{
    Serial.begin(9600);
    Serial.println("Michaels Sand Table Starting Up");
    timer.setInterval(15000, RepeatTask);
    //  pinMode(pushButton, INPUT_PULLUP);

    //TMC2130
    // TMC2130.begin(); // Initiate pins and registeries
    // TMC2130.SilentStepStick2130(600); // Set stepper current to 600mA
    // TMC2130.stealthChop(1); // Enable extremely quiet stepping
    // TMC2130.shaft_dir(0);
    //TMC2130

    radiusArm.Setup();
    thetaArm.Setup();
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
    // state = 17;
    switch (state)
    {
    case 17:
        radiusArm.Startup();
        thetaArm.Startup();
        break;
    case DELAYONBOOT:
        delay(1000);
        state++;
    case STARTUP:
        if (radiusArm.Startup() && thetaArm.Startup() )
        {               //radiusArm has hit a limitSwitch and thetaArmFound a Sensor
            state += 1; //move on to calibration
            Serial.println("State Entering `CALIBRATION` Mode...");
        }
        break;

    case CALIBRATION:
        if ( radiusArm.Calibrate_R_Axis() && thetaArm.Calibrate_Theta_Axis() )//need to check if already calibrated
        {
            state += 1;
            Serial.println("State Entering `USERINPUT` Mode...");
        }
        break;

    case USERINPUT:
        // if there's any serial available, read it:
        while (Serial.available() > 0)
        {
            // look for the next valid integer in the incoming serial stream:
            int steps = Serial.parseInt();
            int secondSteps = Serial.parseInt();
            // look for the newline. That's the end of your sentence:
            if (Serial.read() == '\n')
            {
                Serial.print("Requested Steps: ");
                Serial.println(steps);
                Serial.print("Theta Steps: ");
                Serial.println(secondSteps);
                radiusArm.SetDestination((long)steps);
                thetaArm.SetDestination((long)secondSteps);
            }
        }
        radiusArm.ArmLoop();
        thetaArm.ArmLoop();
        break;
    default:
        state = 0;
        Serial.println("Default Catch `state` var in unkown State: " + String(state));
        radiusArm.DisableMotor();
        while (true)
        {
        } //stop everything here.

    } //switch on state

} //loop
