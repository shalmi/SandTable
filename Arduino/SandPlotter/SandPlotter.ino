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
const int innerLimit = 14;
const int outerLimit = 15;
const int pushButton = 14;
const int yEnable = 56;  //A2
const int yStepPin = 60; //A6
const int yDirPin = 61;  //A7
const int yCsPin = 62; //A8
// const int stepsForRevolution = 3200;

int state = STARTUP;

SimpleTimer timer;

RadiusArm radiusArm = RadiusArm(yDirPin, yEnable, yStepPin, innerLimit, outerLimit); //Works for Normal Driver
// RadiusArm radiusArm = RadiusArm(yDirPin, yEnable, yStepPin, innerLimit, outerLimit, yCsPin); //For TMC2130

// Stepper theta_stepper = Stepper(yDirPin,yEnable,yStepPin);

// #include <TMC2130Stepper.h>
// TMC2130Stepper TMC2130 = TMC2130Stepper(yEnable, yDirPin, yStepPin, yCsPin);

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
        {               //radiusArm has hit a limitSwitch
            state += 1; //move on to calibration
            Serial.println("State Entering `CALIBRATION` Mode...");
        }
        break;

    case CALIBRATION:
        if (radiusArm.Calibrate_R_Axis())
        {
            state += 1;
            radiusArm.DisableMotor();
        }
        break;

    case USERINPUT:
        // if there's any serial available, read it:
        while (Serial.available() > 0)
        {
            // look for the next valid integer in the incoming serial stream:
            int steps = Serial.parseInt();

            // look for the newline. That's the end of your sentence:
            if (Serial.read() == '\n')
            {
                Serial.print("Requested Steps: ");
                Serial.println(steps);
                radiusArm.SetDestination((long)steps);
            }
        }
        radiusArm.ArmLoop();
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
