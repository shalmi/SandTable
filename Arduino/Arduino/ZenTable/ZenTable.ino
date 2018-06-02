#include <MultiStepper.h>
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::FULL2WIRE,A6,A7);
int pushButton = 14;
int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 8000; //steps/second/second to accelerate
int count = 0;
int speed = 32000;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);      // open the serial port at 9600 bps:
  Serial.print("Hello :) ");
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(A2,OUTPUT); // Enable
  digitalWrite(A2,LOW); // Set Enable low
  stepper.setMaxSpeed(speed);
  stepper.setSpeed(speed); 
    
}

void loop() {
    
    digitalWrite(A2,LOW); // Set Enable low
// read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:
//  Serial.println(buttonState);
//  delay(100);        // delay in between reads for stability
  count++;
  if(buttonState == 1){
    if(count>500){
      speed=speed+500;
      stepper.setMaxSpeed(speed);
      stepper.setSpeed(speed); 
      count =0;
      Serial.print(speed);
    }
  }

  
    if (stepper.distanceToGo() == 0){  // Random change to speed, position and acceleration
    // Make sure we dont get 0 speed or accelerations
//    delay(1000);

//    if(buttonState == 0){
//      stepper.forward();
////      stepper.move(200);
//      
//    }
//    else{
//      stepper.backward();
////      stepper.move(-200);
//    }

//    stepper.setMaxSpeed(1000);
//    stepper.setAcceleration(8000);
  }

//Serial.println(stepper.distanceToGo());
//stepper.run();  // Actually makes stepper move
stepper.runSpeed();
  }
