#include "Arduino.h"
#include "Stepper.h"

const uint8_t fan_pin = 2;

Stepper::Stepper(){
}

void Stepper::SETUP(){
  pinMode(fan_pin,OUTPUT);
  digitalWrite(fan_pin,HIGH);
}

void Stepper::SPEED(int fan_speed){

}

int Stepper::rotation(){
  int returnval = 12;
  return (returnval);
}


