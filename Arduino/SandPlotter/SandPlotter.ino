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

double majorPointIndex = -1;
// int majorPointArraySize = 5;
// float arrayMajorXs[] = {500,750, 500, 250, 500};
// float arrayMajorYs[] = {500,500, 750, 500, 250};
// float arrayMajorXs[] = {0,250, 0, -250, 0};
// float arrayMajorYs[] = {0,0, 250, 0, -250};

// int majorPointArraySize = 3;
// float arrayMajorYs[] = {100,100,100};
// float arrayMajorXs[] = {0,250, -250};

// int majorPointArraySize = 9;
// float arrayMajorXs[] = {100,100,100,100,100,100,100,100,100};
// float arrayMajorYs[] = {0,250, -250,250, -250,250, -250,250, -250};

int majorPointArraySize = 7;
float arrayMajorXs[] = {100,100,100,-100,-100,100,100};
float arrayMajorYs[] = {0,100,-100,-100,100,100,0};

double xAndYOffset = 0; //500;

bool radiusArmReady = false;
bool thetaArmReady = false;

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

void findArrayPointsBetweenPoints(float x1,float y1,float x2, float y2){
    /*
    Takes in two Major points and calculates the values needed
    to find all of the major points between them.
    */
    float xDistance = x2-x1;
    float yDistance = y2-y1;
    totalDistance = sqrt( sq(yDistance)+sq(xDistance) );
    totalMinorPoints = totalDistance; //choppy at /5
    xDeltaBy = xDistance/totalMinorPoints;
    yDeltaBy = yDistance/totalMinorPoints;
    nextMinorX = x1;
    nextMinorY = y1;
    currentMinorIndex = 0;
}

void CartesianToPolar(float x,float y, float *R, float *Theta)
{
    // basic math equations for calculating r and theta
    // hopefully this doesnt take TOO long on an arduino.
    // I would like to be able to have these calcs on the arduino itself.

    // assume that xCoord and yCoord are based on a 1000x1000 field
    // where the top left is 0,0
    // This should set 500,500 to be 0,0


    x -= xAndYOffset;
    y -= xAndYOffset;

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
            // state +=1; //1 to go to userinput...2 to cartesian
            state =6;
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
        radiusArmReady = (radiusArm.ArmLoop() == 1);
        thetaArmReady = (thetaArm.ArmLoop() == 1);

        // If both Motors are already at Destination
        if (radiusArmReady && thetaArmReady){
            // Set majorPointIndex to next variable
            majorPointIndex++;

            // Convert Cartesian to Polar
            CartesianToPolar(arrayMajorXs[(int)majorPointIndex],arrayMajorYs[(int)majorPointIndex],&nextMajorR, &nextMajorTheta);

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
        if (radiusArmReady && thetaArmReady){
            // if ball is not out of minor points:
            if (currentMinorIndex <totalMinorPoints){
                currentMinorIndex++;
                
                //calculate next minor point to go to
                nextMinorX+=xDeltaBy;
                nextMinorY+=yDeltaBy;

                // Convert Cartesian to Polar
                CartesianToPolar(nextMinorX,nextMinorY,&nextMajorR, &nextMajorTheta);

                // Set Next Major Destination
                radiusArm.SetDestinationAsCalculatedR(nextMajorR);
                thetaArm.SetDestinationAsCalculatedRadians(nextMajorTheta);

            }
            else{ // if we are ready for the next Major Point

                // Set majorPointIndex to next variable
                majorPointIndex++;
                if (majorPointIndex >= majorPointArraySize){
                    state = 99; //this is nothing
                    break;
                }
                lastMajorX = nextMajorX;
                lastMajorY = nextMajorY;
                nextMajorX = arrayMajorXs[(int)majorPointIndex];
                nextMajorY = arrayMajorYs[(int)majorPointIndex];
                
                // Calculate everything for future minor points till next Major
                findArrayPointsBetweenPoints(lastMajorX,lastMajorY,nextMajorX,nextMajorY);

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
        while (true)
        {
        } //stop everything here.

    } //switch on state

} //loop
