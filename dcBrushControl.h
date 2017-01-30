#ifndef __DC_BRUSH_CONTROL_H
#define __DC_BRUSH_CONTROL_H

#include<convolution.h>
#include<cwccwControl.h>
#include<sensingModule.h>

#define THRESHOLD_HOME	0
#define THRESHOLD_LIMIT	1
#define THRESHOLD_ALARM 2
#define TOTAL_THRESHOLDS 3

#define CHECKPOINT_PRE_LIMIT	0
#define CHECKPOINT_PRE_HOME		1
#define TOTAL_CHECKPOINTS 2

// the mode to output control command to ampfiler IC
#define MODE_CWCCW_PWM 0
#define MODE_PWM_DIRECTION 1

struct dcBrushConfiguration
{
	//---------------------------
	//	Used to configure DC brush motor conrol
	//---------------------------

	//-----------------------
	//	PWM Output Configurations
	//-----------------------
	int outputMode;
	unsigned char outputPin1;		// pwm/cw 
	unsigned char outputPin2;		// direction/ccw
	//------------------------------------
	//	Position Calculation/Control Logic
	//------------------------------------
	int(*voltageSenseSource)();		// link the external sensingSource
	//-----------------------
	//	Command Smoothing
	//-----------------------
	BUFFER_TYPE* smoothFiler;
	int smoothFilterLength;
	//----------------
	//	Control Logic use
	//----------------
	//unsigned short Thresholds[TOTAL_THRESHOLDS];		// recording total thresholds value
	//unsigned int CheckPoints[TOTAL_CHECKPOINTS]; 				// recording the check points
};

typedef struct dcBrushConfiguration DCBRUSHCONFIGURATION;

class dcBrushControl
{
public:
	//dcBrushControl(DCBRUSHCONFIGURATION __config);

	int initialize(DCBRUSHCONFIGURATION __config);

	int drive();		// put under timer task
	
	int pulseValue;		//(input) the un-smoothed voltage command
	float getCurrentPosition(){ return positionMonitor.getCurrentPosition(); }	//(output) the current position 
	float getCurrentSpeed(){ return positionMonitor.getCurrentSpeed(); }		//(output) voltage difference

	void clearCurrentPosition(){ positionMonitor.clearCurrentPosition(); };
//private:

	int sensingValue;
	int smoothedValue;	// the smoothed value , had filtered by convolution
	//float currentPosition;	//in abs
	int(*voltageSenseSource)();		// link the external sensingSource

	//cwccwControl outputModule;
	dcMotorOutputBase* outputModule;
	convolution* commandSmoother;
	sensingModule positionMonitor;
};

#endif