/*     SandPlotter
 *      
 *  by Michael Yeroshalmi
 *  
 */



// Michaels Sand Table Starting Up
// RadiusArm Startup Finished
// HallEffect Calibration at: 215
// ThetaArm Startup Finished
// State Entering `CALIBRATION` Mode...
// Steps between switches = 17451
// Full Distance of Steps for Theta: 152619
// State Entering `USERINPUT` Mode...

#include "Arduino.h"
#include "Stepper.h"
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
float nextMajorTheta = 0;
float nextMajorR = 0;
float previousMajorTheta = 0;
float previousMajorR = 0;

float nextMajorX = 0.0;
float nextMajorY = 0.0;
float lastMajorX = 0.0;
float lastMajorY = 0.0;

double majorPointIndex = 0;

float arrayMajorXs[] = {2, 4, 8, 3, 6};
float arrayMajorYs[] = {2, 4, 8, 3, 6};

float *pointerToR;
float *pointerToTheta;


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

void CartesianToPolar(float x,float y, float *R, float *Theta)
{
    // basic math equations for calculating r and theta
    // hopefully this doesnt take TOO long on an arduino.
    // I would like to be able to have these calcs on the arduino itself.

    // assume that xCoord and yCoord are based on a 1000x1000 field
    // where the top left is 0,0
    // This should set 500,500 to be 0,0

    x -= 500;
    y -= 500;

    if(x == 0 && y == 0 ){
        *R = 0;
        *Theta = 0;
        return;
    }
    *R = sqrt( sq(x) + sq(y) );

    if(x == 0 && 0 < y){
        *Theta = PI/2.0;
    }else if(x == 0 && y < 0){
        *Theta = PI*3.0/2.0;
    }else if(x < 0){ //x != 0
        *Theta = atan(y/x) + PI;
    }else if (y < 0){
        *Theta = atan(y/x) + 2.0*PI;
    }else{
        *Theta = atan(y/x);
    }
}


void loop()
{   
    // state = 17;
    switch (state)
    {
    // case 17:
    //     radiusArm.Startup();
    //     thetaArm.Startup();
        break;
    case DELAYONBOOT:
        // delay(1000);
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
            state += 2; //1 to go to userinput...2 to cartesian
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
    case USERINPUTCARTESIAN:
        // if there's any serial available, read it:
        while (Serial.available() > 0)
        {
            // look for the next valid integer in the incoming serial stream:
            int xCoord = Serial.parseInt();
            int yCoord = Serial.parseInt();
            // look for the newline. That's the end of your sentence:
            if (Serial.read() == '\n')
            {
                Serial.print("x: ");
                Serial.println(xCoord);
                Serial.print("y: ");
                Serial.println(yCoord);

                // Set Old Values to = Last new values
                previousMajorTheta = nextMajorTheta;
                previousMajorR = nextMajorR;

                CartesianToPolar((float)xCoord,(float)yCoord, &nextMajorR, &nextMajorTheta);
                Serial.print("R: ");
                Serial.println(nextMajorR);
                Serial.print("theta: ");
                Serial.println(nextMajorTheta);
                radiusArm.SetDestinationAsCalculatedR(nextMajorR);
                thetaArm.SetDestinationAsCalculatedRadians(nextMajorTheta);


                // radiusArm.SetDestination((long)steps);
                // thetaArm.SetDestination((long)secondSteps);
            }
        }
        radiusArm.ArmLoop();
        thetaArm.ArmLoop();
        break;
    case 5: // Go through a set of Major Points.
        
        // Inform me when there
        // each bool == True when that motor is waiting on a new command
        bool radiusArmReady = (radiusArm.ArmLoop() == 1);
        bool thetaArmReady = (thetaArm.ArmLoop() == 1);

        // If both Motors are already at Destination
        if (radiusArmReady && thetaArmReady){
            // Set majorPointIndex to next variable
            majorPointIndex++;

            // Convert Cartesian to Polar
            CartesianToPolar(arrayMajorXs[majorPointIndex],arrayMajorYs[majorPointIndex],&nextMajorR, &nextMajorTheta);

            // Set Next Major Destination
            radiusArm.SetDestinationAsCalculatedR(nextMajorR);
            thetaArm.SetDestinationAsCalculatedRadians(nextMajorTheta);

        }
    

    default:
        state = 0;
        Serial.println("Default Catch `state` var in unkown State: " + String(state));
        radiusArm.DisableMotor();
        while (true)
        {
        } //stop everything here.

    } //switch on state

} //loop
