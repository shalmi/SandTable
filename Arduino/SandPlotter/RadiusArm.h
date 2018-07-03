#ifndef RadiusArm_h
#define RadiusArm_h

#include "Stepper.h"

class RadiusArm
{
  private:
	int speed;
	int innerEndStop;
	int outerEndStop;
	Stepper stepper;
	// const int directionPin;
	// const int enablePin;
	// const int stepPin;

  public:
	RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop);
	void Setup();
	void ChangeDirection(bool newDirection);
	bool Startup();
	bool ReverseDirectionOnBump();
	void Calibrate_R_Axis();
};

// extern RadiusArm fan;

#endif
