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
    void Init(int _directionPin, int _enablePin, int _stepPin, int _chipSelectPin);
    void SetSpeed(int _speed);
    void OneStep();
    void ChangeDirection(bool newDirection);
    void DisableMotor();
    void EnableMotor();
};

// extern Stepper fan;

#endif
