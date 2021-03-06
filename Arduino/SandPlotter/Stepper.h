#ifndef Stepper_h
#define Stepper_h

class Stepper
{
  private:
    int speed;
    int directionPin;
    int enablePin;
    int stepPin;
    unsigned long timeAtNextPulse;
    unsigned long timeToDisableMotor;
    bool currentlyMidStep = false;
    bool motorEnabled = false;

  public:
    Stepper();
    void Init(int _directionPin, int _enablePin, int _stepPin, int _speed);
    void Init(int _directionPin, int _enablePin, int _stepPin, int _speed, int _chipSelectPin);
    void Startup();
    void SetSpeed(int _speed);
    //Takes a full Step
    //currently goes at 50*speed (higher is slower)
    //speed is microseconds between high and low triggers.
    bool OneStepIfTime();
    void OneStep();
    void ChangeDirection(bool newDirection);
    void DisableMotor();
    void EnableMotor();
    void DisableMotorIfBored();
};

// extern Stepper fan;

#endif
