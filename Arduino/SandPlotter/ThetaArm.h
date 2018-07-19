#ifndef ThetaArm_h
#define ThetaArm_h

#include "Stepper.h"

/**
* COMENT OF A CLASS
**/
class ThetaArm
{
  private:
	#define IDLE (0)
	#define GO_TO_POINT (1)
	int speed;
	int armState;
	int hallPin; //Pin for Hall Effect Sensor
	Stepper stepper;
	void TakeStep();
	bool RoomToMove();
	void EnableMotor();
	bool MoveTowardsDestination();
	void ChangeDirection(bool newDirection);


	const bool clockWise = true;
	const bool counterClockWise = false;
	bool currentDirection = counterClockWise;
	long desiredLocation = 0;
	long currentLocation = 0;
	long stepCounter = 0;
	long fullDistance;
	const bool HIT = true;
	const bool NOT_HIT = false;
	// const int directionPin;
	// const int enablePin;
	// const int stepPin;

  public:
	ThetaArm(int directionPin, int enablePin, int stepPin, int _hallPin);
	void Setup();
	bool Startup();
	bool ReverseDirectionOnBump();
	bool Calibrate_Theta_Axis();
	void DisableMotor();
	void SetDestination(long destination);
	void ArmLoop();
};

// extern RadiusArm fan;

#endif