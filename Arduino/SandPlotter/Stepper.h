#ifndef Stepper_h
#define Stepper_h

class Stepper
{
  private:
    int speed;
    int directionPin;
    int enablePin;
    int stepPin;

  public:
    Stepper();
    void Init(int _directionPin, int _enablePin, int _stepPin);
    void SetSpeed(int _speed);
    void OneStep();
    void ChangeDirection(bool newDirection);
};

// extern Stepper fan;

#endif
