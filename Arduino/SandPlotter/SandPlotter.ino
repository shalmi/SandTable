/*     SandPlotter
 *      
 *  by Michael Yeroshalmi
 *  
 */

// change to 20k
// also you need to set some sort of pid for time and speed...
// also need to rework 3d model for  theta...like a lot

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

#include "microSD.h"
// #include "FS.h"
// #include "SD.h"
// #include "SPI.h"

//Add 54 to num
// static const uint8_t A0 = 54;

////////////ARDUINO MEGA//////////////
// // defines pins numbers
// const int innerLimit = 14;
// const int outerLimit = 15;
// const int pushButton = 14;
// //RadiusMotorPINS
// const int rEnable = 56;  //A2
// const int rStepPin = 60; //A6
// const int rDirPin = 61;  //A7
// const int rCsPin = 62; //A8
// //ThetaMotorPINS
// const int thetaDirPin = 63;  //A9
// const int thetaStepPin = 64; //A10
// const int thetaEnable = 65;  //A11
// const int thetaHallPin = 66; //THIS IS NOT TRUE....shhhh
////////////ARDUINO MEGA//////////////

////////////Feather ESP32//////////////
// had to make changes to the hall effect sensor in thetaarm.cpp. plus magnet sensed value in thetaarm.h
//also the pullup resisters dont seem to work for shit at all...WHAT THE FUCK
// defines pins numbers
const int innerLimit = 39; //this is an analog input A3 and also GPI #39. Note it is not an output-capable pin! It uses ADC #1
const int outerLimit = 36; //this is an analog input A4 and also GPI #36. Note it is not an output-capable pin! It uses ADC #1
//RadiusMotorPINS
const int rEnable = 15;  //This is GPIO #15 and also an analog input A8 on ADC #2
const int rStepPin = 32; //This is GPIO #32 and also an analog input A7 on ADC #1. It can also be used to connect a 32 KHz crystal.
const int rDirPin = 14;  //This is GPIO #14 and also an analog input A6 on ADC #2
// const int rCsPin = 62; //A8
//ThetaMotorPINS
const int thetaDirPin = 27;  //This is GPIO #27 and also an analog input A10 on ADC #2
const int thetaStepPin = 33; //This is GPIO #33 and also an analog input A9 on ADC #1. It can also be used to connect a 32 KHz crystal.
const int thetaEnable = 21;  //General purpose IO pin #21
const int thetaHallPin = 34; // this is an analog input A2 and also GPI #34. Note it is not an output-capable pin! It uses ADC #1
////////////Feather ESP32//////////////

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

float nextMinorTheta = 0;
float nextMinorR = 0;

float nextMinorX = 0.0;
float nextMinorY = 0.0;
float lastMinorX = 0.0;
float lastMinorY = 0.0;
float currentMinorX = 0;
float currentMinorY = 0;
float xDeltaBy = 0.0;
float yDeltaBy = 0.0;
unsigned int totalMinorPoints = 0;
unsigned int currentMinorIndex = 0;
double totalDistance = 0;

int majorPointIndex = -1; //this can only handle so many points
// int majorPointArraySize = 5;
// float arrayMajorXs[] = {500,750, 500, 250, 500};
// float arrayMajorYs[] = {500,500, 750, 500, 250};
// float arrayMajorXs[] = {0,250, 0, -250, 0};
// float arrayMajorYs[] = {0,0, 250, 0, -250};

// int majorPointArraySize = 3;
// float arrayMajorYs[] = {100,100,100};
// float arrayMajorXs[] = {0,250, -250};

int majorPointArraySize = 9;
float arrayMajorXs[] = {100, 100, 100, 100, 100, 100, 100, 100, 100};
float arrayMajorYs[] = {0, 250, -250, 250, -250, 250, -250, 250, -250};

// int majorPointArraySize = 7;
// float arrayMajorXs[] = {100,100,100,-100,-100,100,100};
// float arrayMajorYs[] = {0,100,-100,-100,100,100,0};

// int majorPointArraySize = 6;
// float arrayMajorXs[] = {200,-200,200,-200,200,-200};
// float arrayMajorYs[] = {0,0,0,0,0,0};

double xAndYOffset = 0; //500;

bool radiusArmReady = false;
bool thetaArmReady = false;

RadiusArm radiusArm = RadiusArm(rDirPin, rEnable, rStepPin, innerLimit, outerLimit); //Works for Normal Driver
ThetaArm thetaArm = ThetaArm(thetaDirPin, thetaEnable, thetaStepPin, thetaHallPin);
// RadiusArm radiusArm = RadiusArm(rDirPin, rEnable, rStepPin, innerLimit, outerLimit, rCsPin); //For TMC2130

// Stepper theta_stepper = Stepper(rDirPin,rEnable,rStepPin);

// #include <TMC2130Stepper.h>
// TMC2130Stepper TMC2130 = TMC2130Stepper(rEnable, rDirPin, rStepPin, rCsPin);

TaskHandle_t Task1; //This is the Core Zero Task

void setup()
{
    digitalWrite(65, HIGH); // disable motor
    digitalWrite(56, HIGH); // disable motor
    Serial.begin(115200);
    Serial.print("This is the main loop which runs on Core #");
    Serial.println(xPortGetCoreID());
    //start other Convert
    xTaskCreatePinnedToCore(
        mainLoopForCoreZero,       // Task function
        "nameOfCoreZeroTaskMaybe", // name of task. Can show up in cpu crashes
        10000,                     //Stack size of task. This was tripping all kinds of errors for me.
        NULL,                      //Parameter of the task
        1,                         //priority of the task. I need to fiddle with this and make sure it does not get in the way of the main loop
        &Task1,                    // Task handle to keep track of created task
        0);                        // Which core to attach this to. Core 1 is the main loop

    delay(1000); // needed to start-up task 1

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

void findArrayPointsBetweenPoints(float x1, float y1, float x2, float y2)
{
    /*
    Takes in two Major points and calculates the values needed
    to find all of the major points between them.
    */
    float xDistance = x2 - x1;
    float yDistance = y2 - y1;
    totalDistance = sqrt(sq(yDistance) + sq(xDistance));
    totalMinorPoints = totalDistance; //choppy at /5
    // /3 = skips between 48 and 51
    // /3 = skips between 52 and 55
    // /2 = skipps between 55 and 60
    // *1 = 61
    // *2 = 65
    // *3 = 67
    // *4 = 70

    xDeltaBy = xDistance / totalMinorPoints;
    yDeltaBy = yDistance / totalMinorPoints;
    nextMinorX = x1 + xDeltaBy;
    nextMinorY = y1 + yDeltaBy;
    currentMinorIndex = 1;
}
// I dont think this code works ...it led the end to be in the wrong SandPlotter
// void findArrayPointsBetweenPoints(float x1,float y1,float x2, float y2){
//     /*
//     Takes in two Major points and calculates the values needed
//     to find all of the major points between them.
//     */
//     float xDistance = x2-x1;
//     float yDistance = y2-y1;
//     float absXDist  = abs(xDistance);
//     float absYDist = abs(yDistance);
//     if (absXDist > absYDist){
//         totalMinorPoints = absXDist;
//     }
//     else{
//         totalMinorPoints = absYDist;
//     }
//     totalMinorPoints=totalMinorPoints;
//     xDeltaBy = xDistance/totalMinorPoints;
//     yDeltaBy = yDistance/totalMinorPoints;
//     nextMinorX = x1;
//     nextMinorY = y1;
//     currentMinorIndex = 0;
// }

void CartesianToPolar(float x, float y, float *R, float *Theta)
{
    // basic math equations for calculating r and theta
    // hopefully this doesnt take TOO long on an arduino.
    // I would like to be able to have these calcs on the arduino itself.

    // assume that xCoord and yCoord are based on a 1000x1000 field
    // where the top left is 0,0
    // This should set 500,500 to be 0,0

    x -= xAndYOffset;
    y -= xAndYOffset;

    if (x == 0 && y == 0)
    {
        *R = 0;
        *Theta = 0;
        return;
    }
    *R = sqrt(sq(x) + sq(y));

    if (x == 0 && 0 < y)
    {
        *Theta = PI / 2.0;
    }
    else if (x == 0 && y < 0)
    {
        *Theta = PI * 3.0 / 2.0;
    }
    else if (x < 0)
    { //x != 0
        *Theta = atan(y / x) + PI;
    }
    else if (y < 0)
    {
        *Theta = atan(y / x) + 2.0 * PI;
    }
    else
    {
        *Theta = atan(y / x);
    }
}

void loop()
{
    // pop all the string messages from the queue.
    while (!queue.isEmpty()){
        gCodeStruct coord = queue.pop();
        Serial.print("\nX: ");
        Serial.print(coord.x);
        Serial.print(",Y: ");
        Serial.print(coord.y);
}

    // Serial.println("main loop still running!");
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
        if (radiusArm.Startup() && thetaArm.Startup())
        {               //radiusArm has hit a limitSwitch and thetaArmFound a Sensor
            state += 1; //move on to calibration
            Serial.println("State Entering `CALIBRATION` Mode...");
        }
        break;

    case CALIBRATION:
        if (radiusArm.Calibrate_R_Axis() && thetaArm.Calibrate_Theta_Axis()) //need to check if already calibrated
        {
            // state +=1; //1 to go to userinput...2 to cartesian
            state = 6;
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

                CartesianToPolar((float)xCoord, (float)yCoord, &nextMajorR, &nextMajorTheta);
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
        radiusArmReady = (radiusArm.ArmLoop() == 1);
        thetaArmReady = (thetaArm.ArmLoop() == 1);

        // If both Motors are already at Destination
        if (radiusArmReady && thetaArmReady)
        {
            // Set majorPointIndex to next variable
            majorPointIndex++;

            // Convert Cartesian to Polar
            CartesianToPolar(arrayMajorXs[(int)majorPointIndex], arrayMajorYs[(int)majorPointIndex], &nextMajorR, &nextMajorTheta);

            // Set Next Major Destination
            radiusArm.SetDestinationAsCalculatedR(nextMajorR);
            thetaArm.SetDestinationAsCalculatedRadians(nextMajorTheta);
            Serial.print("NEXT POINT: ");
            Serial.print(nextMajorR);
            Serial.print(", ");
            Serial.println(nextMajorTheta);
        }
        break;
    case 6: // Go through a set of Major Points IN A STRAIGHT LINE.

        // Inform me when there
        // each bool == True when that motor is waiting on a new command
        radiusArmReady = (radiusArm.ArmLoop() == 1);
        thetaArmReady = (thetaArm.ArmLoop() == 1);

        // If both Motors are already at Destination
        if (radiusArmReady && thetaArmReady)
        {
            // if ball is not out of minor points:
            if (currentMinorIndex < totalMinorPoints)
            {
                currentMinorIndex++;
                radiusArm.SetSpeed(15);
                thetaArm.SetSpeed(60);

                //calculate next minor point to go to
                nextMinorX += xDeltaBy;
                nextMinorY += yDeltaBy;

                // Convert Cartesian to Polar
                CartesianToPolar(nextMinorX, nextMinorY, &nextMajorR, &nextMajorTheta);

                // Set Next Major Destination
                radiusArm.SetDestinationAsCalculatedR(nextMajorR);
                thetaArm.SetDestinationAsCalculatedRadians(nextMajorTheta);
            }
            else
            { // if we are ready for the next Major Point

                // Set majorPointIndex to next variable
                majorPointIndex++;
                if (majorPointIndex >= majorPointArraySize)
                {
                    state = 99; //this is nothing
                    break;
                }
                lastMajorX = nextMajorX;
                lastMajorY = nextMajorY;
                nextMajorX = arrayMajorXs[majorPointIndex];
                nextMajorY = arrayMajorYs[majorPointIndex];

                // Calculate everything for future minor points till next Major
                findArrayPointsBetweenPoints(lastMajorX, lastMajorY, nextMajorX, nextMajorY);

                // Ball should already be at first point...so no reason to go there.

                Serial.print("NEXT POINT: ");
                Serial.print(nextMajorX);
                Serial.print(", ");
                Serial.println(nextMajorY);
            }
        }
        break;

    default:
        state = 0;
        Serial.println("Default Catch `state` var in unkown State: " + String(state));
        radiusArm.DisableMotor();
        thetaArm.DisableMotor();
        while (true)
        {
        } //stop everything here.

    } //switch on state

} //loop

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Code for Core Zero~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void mainLoopForCoreZero(void *parameter)
{
    Serial.print("This is the 2nd loop which runs on Core #");
    Serial.println(xPortGetCoreID());
    otherCoreLoop();
}