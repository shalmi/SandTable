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
	bool OnHallEffect();

	const bool clockWise = false;
	const bool counterClockWise = true;
	bool currentDirection = clockWise;
	long desiredLocation = 0;
	long currentLocation = 0;
	long stepCounter = 0;
	// long fullDistance;
	const bool HIT = true;
	const bool NOT_HIT = false;
	int hallValue = 512; //set to nothing pretty much
	bool startupFinished = false;
	bool calibrationFinished = false;
	int magnetSensedValue = 500;
	int hallEffectCalibrationCounter = 0;
	long fullDistance = 0;
	const long TheoreticalStepsInRotation = 8200;
	long stepsInRotationOverTwoPi = 0;
	const float pi = 3.1415926;
	// const int directionPin;
	// const int enablePin;
	// const int stepPin;

  public:
	ThetaArm(int directionPin, int enablePin, int stepPin, int _hallPin);
	void Setup();
	bool Startup();
	bool CalibrateHallEffectSensor();
	bool Calibrate_Theta_Axis();
	void DisableMotor();
	void SetDestination(long destination);
	void SetDestinationAsCalculatedRadians(float nextMajorTheta);
	void ArmLoop();
};

// extern RadiusArm fan;

#endif
