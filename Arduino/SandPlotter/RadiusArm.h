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
	int innerEndStop; //Both Switches are shorted when the switch is Open
	int outerEndStop; //and it is an open circuit when the switch is Hit.
	Stepper stepper;
	bool TakeStep();
	bool RoomToMove();
	void Live();
	bool MoveTowardsDestination();
	void ChangeDirection(bool newDirection);

	const bool HIT = true;
	const bool NOT_HIT = false;
	const bool outward = true;
	const bool inward = false;
	long stepCounter = 0;
	float fullDistOverRange;
	long centerOffset = 6800;
	long fullDistance;
	bool currentDirection = outward;
	long desiredLocation = 0;
	long currentLocation = 0;
	bool startupFinished = false;
	bool calibrationFinished = false;
	// const int directionPin;
	// const int enablePin;
	// const int stepPin;

  public:
	RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop);
	RadiusArm(int directionPin, int enablePin, int stepPin, int _innerEndStop, int _outerEndStop, int _chipSelectPin);
	void Setup();
	bool Startup();
	bool ReverseDirectionOnBump();
	bool Calibrate_R_Axis();
	void DisableMotor();
	void SetDestination(long destination);
	void SetDestinationAsCalculatedR(float destination);
	void ArmLoop();
};

// extern RadiusArm fan;

#endif
