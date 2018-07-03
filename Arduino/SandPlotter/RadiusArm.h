#ifndef RadiusArm_h
#define RadiusArm_h

// #include "Stepper.h"

class RadiusArm
{
  private:
    int speed;
    // const int directionPin;
    // const int enablePin;
    // const int stepPin;
    // Stepper stepper;
  
  public:
    RadiusArm(int directionPin,int enablePin,int stepPin);
    void Setup();
    void Setspeed(int _speed);
    void OneStep();
    void ChangeDirection(bool newDirection);
};

// extern RadiusArm fan;

#endif
