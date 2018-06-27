/*     SandPlotter
 *      
 *  by Michael Yeroshalmi
 *  
 */

#include "Arduino.h"
#include "Stepper.h"

Stepper fan = Stepper();

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
const int stepsForRevolution = 3200;
const bool outward = true;
const bool inward = false;
bool currentDirection = outward;
const bool HIT = true;
const bool NOT_HIT = false;

void setup() {
  fan.SETUP();

  Serial.begin(9600);
  // Sets the two pins as Outputs
  pinMode(yStepPin,OUTPUT); 
  pinMode(yDirPin,OUTPUT);
  pinMode(yEnable,OUTPUT); // Enable
//  pinMode(pushButton, INPUT_PULLUP);
  pinMode(innerLimit, INPUT_PULLUP);
  pinMode(outerLimit, INPUT_PULLUP);
  digitalWrite(yEnable,LOW); // Set Enable low
}

void OneStep(int pin, int stepperSpeed){
    stepperSpeed = stepperSpeed*50;
    digitalWrite(pin,HIGH); 
    delayMicroseconds(stepperSpeed); 
    digitalWrite(pin,LOW); 
    delayMicroseconds(stepperSpeed); 
}
void SetDirection(){
  if(digitalRead(outerLimit) == HIT){
    currentDirection = inward;
  }
  if(digitalRead(innerLimit) == HIT){
    currentDirection = outward;
  }
  digitalWrite(yDirPin,currentDirection); // Enables the motor to move in a particular direction
}
// void InitialCalibration(){

// }

void loop() {

  // //sets a direction and takes a step
  // SetDirection();//checks limit switches and changes direction if Hit
  // OneStep(yStepPin,1);

}