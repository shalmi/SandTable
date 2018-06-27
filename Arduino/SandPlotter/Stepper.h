#ifndef Stepper_h
#define Stepper_h

class Stepper
{
  public:
    Stepper(int directionPin,int enablePin,int stepPin);
    void Setup();
    void SetSpeed(int speed);
    int rotation();
};

// extern Stepper fan;

#endif
