/*     SandPlotter
 *      
 *  by Michael Yeroshalmi
 *  
 */

#include "Arduino.h"
#include "Stepper.h"
#include "SimpleTimer.h"


//Add 54 to num
// static const uint8_t A0 = 54;

// defines pins numbers
//const int pushButton = 14;
const int innerLimit = 14;
const int outerLimit = 15;
const int pushButton = 14;
const int yEnable = 56; //A2
const int yStepPin = 60; //A6
const int yDirPin = 61; //A7
// const int stepsForRevolution = 3200;
const bool outward = true;
const bool inward = false;
bool currentDirection = outward;
const bool HIT = true;
const bool NOT_HIT = false;

int state = 0;
long stepCounter = 0;

SimpleTimer timer;

Stepper r_stepper = Stepper(yDirPin,yEnable,yStepPin);
// Stepper theta_stepper = Stepper(yDirPin,yEnable,yStepPin);

void setup() {

  Serial.begin(9600);
  timer.setInterval(15000, RepeatTask);
//  pinMode(pushButton, INPUT_PULLUP);
  pinMode(innerLimit, INPUT_PULLUP);
  pinMode(outerLimit, INPUT_PULLUP);
  r_stepper.Setup();
}

// function to be called repeatedly
void RepeatTask() {
  Serial.println("15 second timer");        
}

void ReverseDirectionOnBump(){
  if((digitalRead(outerLimit) == HIT) && (currentDirection == outward)){
    currentDirection = inward;
    Serial.println(stepCounter);
    stepCounter = 0;
  }
  if((digitalRead(innerLimit) == HIT) && (currentDirection == inward)){
    currentDirection = outward;
    Serial.println(stepCounter);
    stepCounter = 0;
  }
  digitalWrite(yDirPin,currentDirection); // Enables the motor to move in a particular direction
}
// void InitialCalibration(){

// }

void loop() {
  
  // //sets a direction and takes a step
//  r_stepper.ChangeDirection(outward);
  r_stepper.OneStep();
  stepCounter+=1;
  ReverseDirectionOnBump();
//  Serial.println(stepCounter);
  // SetDirection();//checks limit switches and changes direction if Hit
  // OneStep(yStepPin,1);
}
