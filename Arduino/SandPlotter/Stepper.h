#ifndef Stepper_h
#define Stepper_h

class Stepper
{
  public:
    Stepper();
    void SETUP();
    void SPEED(int fan_speed);
    int rotation();
};

extern Stepper fan;

#endif
