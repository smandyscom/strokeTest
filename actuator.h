#ifndef __HOMING
#define	__HOMING
#include "device.h"

#define THRESHOLD_HOME	0
#define THRESHOLD_LIMIT	1
#define THRESHOLD_ALARM 2
#define TOTAL_THRESHOLDS 3

#define CHECKPOINT_PRE_LIMIT	0
#define CHECKPOINT_PRE_HOME		1
#define TOTAL_CHECKPOINTS 2
// commands
#define REQUEST_TO_LIMIT	0x1
#define REQUEST_TO_HOME		0x2
#define REQUEST_STOP		0x4

#define LIMIT_END_REACHED 0x1
#define HOME_END_REACHED 0x2

struct actuator
{
	struct deviceBase base;
	//-----------------------------
	//	Properties
	//-----------------------------
	unsigned short Thresholds[TOTAL_THRESHOLDS];		// recording total thresholds value
	unsigned int CheckPoints[TOTAL_CHECKPOINTS];				// recording the check points
	//int (*Output)(char __direction);									// the output function
	int* OutputPort;
	unsigned int(*CurrentSense)();								// callback
	char Command;										// the command to select working flow in working state
	//----------------------------------
	//	Privates
	//----------------------------------
	unsigned int currentPosition;		// accumulating
	unsigned int limitPosition;			// the constant
	int homeStep;			//	= speed*homeDirection
	int limitStep;			//	= speed*limitDirection
	//char HomeDirection;		// indicate direction to HOME,whether 1 or -1
	//char LimitDirection;		// indicate direction to LIMIT,whether 1 or -1 , oppotiste to HOME
	int speed;

	char status;		// 
};

int actuatorInitialize(actuator*);
float actuatorCurrentDegree(actuator*);	// report current degreed = currentPosition/limitPosition

void actuatorSetDirectionAndSpeed(actuator*, char , int);
//void actuatorSetSpeed(actuator*, int);
#endif // !1
