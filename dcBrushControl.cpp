#ifndef __DC_BRUSH_CONTROL_CPP
#define __DC_BRUSH_CONTROL_CPP

#ifndef NON_ARDUINO_PROJECT
#include<Arduino.h>
#include<HardwareSerial.h>
#endif

#include<dcBrushControl.h>
//#include<convolution.h>
//#include<cwccwControl.h>
//#include<sensingModule.h>
//
//#define THRESHOLD_HOME	0
//#define THRESHOLD_LIMIT	1
//#define THRESHOLD_ALARM 2
//#define TOTAL_THRESHOLDS 3
//
//#define CHECKPOINT_PRE_LIMIT	0
//#define CHECKPOINT_PRE_HOME		1
//#define TOTAL_CHECKPOINTS 2
//
//struct dcBrushConfiguration
//{
//	//---------------------------
//	//	Used to configure DC brush motor conrol
//	//---------------------------
//
//	//-----------------------
//	//	PWM Output Configurations
//	//-----------------------
//	int forwardPin;
//	int backwardPin;
//	//------------------------------------
//	//	Position Calculation/Control Logic
//	//------------------------------------
//	(int)(*voltageSenseSource)();		// link the external sensingSource
//	//-----------------------
//	//	Command Smoothing
//	//-----------------------
//	BUFFER_TYPE* smoothFiler;
//	int smoothFilterLength;
//	//----------------
//	//	Control Logic use
//	//----------------
//	unsigned short Thresholds[TOTAL_THRESHOLDS];		// recording total thresholds value
//	unsigned int CheckPoints[TOTAL_CHECKPOINTS];				// recording the check points
//};
//
//typedef struct dcBrushConfiguration DCBRUSHCONFIGURATION;
//
//class dcBrushControl
//{
//public:
//	dcBrushControl(DCBRUSHCONFIGURATION __config);
//
//	(int)(*voltageSenseSource)();		// link the external sensingSource
//
//	int drive();
//
//private:
//
//	int sensingValue;
//	int pulseValue;		// the un-smoothed voltage command
//	int smoothedValue;	// the smoothed value , had filtered by convolution
//	long currentPosition;	//in abs
//
//	cwccwControl outputModule;
//	convolution* commandSmoother;
//	sensingModule positionMonitor;
//};

int dcBrushControl::initialize(DCBRUSHCONFIGURATION __config)
{
#ifndef NON_ARDUINO_PROJECT
	Serial.println("dc init start");
#endif
	
	memset(this, 0, sizeof(dcBrushControl));

	this->commandSmoother = new convolution(__config.smoothFilterLength , __config.smoothFiler);	//allocating? possible to use?

	switch (__config.outputMode)
	{
	case MODE_CWCCW_PWM:
		this->outputModule = new cwccwControl;
		((cwccwControl*)this->outputModule)->initialize(__config.outputPin1, __config.outputPin2);
		//this->outputModule.initialize(__config.forwardPin, __config.backwardPin);
		break;
	case MODE_PWM_DIRECTION:
		this->outputModule = new pwmDirection;
		((pwmDirection*)this->outputModule)->initialize(__config.outputPin1, __config.outputPin2);
		break;
	default:
		break;
	}
	this->voltageSenseSource = __config.voltageSenseSource;

#ifndef NON_ARDUINO_PROJECT
	Serial.println("dc init over");
#endif
	//currentPosition = 0;
}

int dcBrushControl::drive()
{
	//-----------------------
	//	The control routine 
	//-----------------------
//#ifndef NON_ARDUINO_PROJECT
//	Serial.println("dc run");
//#endif
	//------------------------------------
	//read sensing value into buffer area
	//------------------------------------
	sensingValue = voltageSenseSource();		//get sensing value
	//----------------------------------------------------------------------
	//	inject pulseValue(vCommand) into covolution , get the latest command
	//----------------------------------------------------------------------
	smoothedValue = commandSmoother->convolutionCalculate(pulseValue);
	//----------------------------------------------------------------------
	//	use vCommand,vSense to calculate lastest real position ( in absolution form)
	//----------------------------------------------------------------------
	/*currentPosition =*/ positionMonitor.calculateCurrentPosition(smoothedValue,sensingValue);
	//-------------------------------------
	//	Output the smoothedValue to motor
	//-------------------------------------
	outputModule->drive(smoothedValue);

	// control logic



	return 0;
}

#endif 