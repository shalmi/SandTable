#ifndef RadiusArm_h
#define RadiusArm_h

#include "Stepper.h"

class RadiusArm
{
  private:
	#define IDLE (0)
	#define GO_TO_POINT (1)
	int speed;
	int armState;
	int innerEndStop;
	int outerEndStop;
	Stepper stepper;
	void TakeStep();
	bool RoomToMove();
	void Live();
	bool MoveTowardsDestination();
	void ChangeDirection(bool newDirection);

	// const int directionPin;
	// const int enablePin;
	// const int stepPin;

  public:
	RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop);
	void Setup();
	bool Startup();
	bool ReverseDirectionOnBump();
	bool Calibrate_R_Axis();
	void DisableMotor();
	void SetDestination(long destination);
	void ArmLoop();
};

// extern RadiusArm fan;

#endif