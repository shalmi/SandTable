#ifndef Stepper_h
#define Stepper_h

class Stepper
{
  const int directionPin;
  const int enablePin;
  const int stepPin;
  
  public:
    Stepper(int directionPin,int enablePin,int stepPin);
    void Setup();
    void SetSpeed(int _speed);
    void OneStep();
    void ChangeDirection(bool newDirection);
};

// extern Stepper fan;

#endif
