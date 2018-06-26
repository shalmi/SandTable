/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
//Add 54 to num
// static const uint8_t A0 = 54;
//A1 = 55;
//A2 = 56;
//A2 = 56;
//A3 = 56;
//A4 = 56;
//A5 = 56;
//A6 = 56;
//A7 = 56;
//A8 = 56;
//A9 = 56;
//A10
//A11
//A12
//A13 = 67;
//A14 = 68;
//A15 = 69;

// defines pins numbers
//const int pushButton = 14;
const int innerLimit = 14;
const int outerLimit = 15;
const int pushButton = 14;

const int yEnable = 56; //A2
const int stepPin = 60; //A6
const int dirPin = 61; //A7
const int stepsForRevolution = 3200;
const bool outward = true;
const bool inward = false;
bool currentDirection = outward;
const bool HIT = true;
const bool NOT_HIT = false;

void setup() {
  Serial.begin(9600);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(yEnable,OUTPUT); // Enable
//  pinMode(pushButton, INPUT_PULLUP);
  pinMode(innerLimit, INPUT_PULLUP);
  pinMode(outerLimit, INPUT_PULLUP);
  digitalWrite(yEnable,LOW); // Set Enable low
}

void OneStep(int stepPin, int stepperSpeed){
    stepperSpeed = stepperSpeed*50;
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(stepperSpeed); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(stepperSpeed); 
}
void setDirection(){
  if(digitalRead(outerLimit) == HIT){
    currentDirection = inward;
  }
  if(digitalRead(innerLimit) == HIT){
    currentDirection = outward;
  }
  digitalWrite(dirPin,currentDirection); // Enables the motor to move in a particular direction
}

void loop() {
  setDirection();//checks limit switches and changes direction if Hit
  OneStep(stepPin,1);
//  Serial.print(digitalRead(outerLimit));

//THIS MAKES A COMPLETE LOOP AND STOPS AT A PUSHBUTTON
//  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
//  for(int x = 0; x < stepsForRevolution; x++) {
//    if(digitalRead(pushButton) == 0){
//      OneStep(stepPin,1);
//    }
//  }
//  delay(1000); // One second delay
  
}
