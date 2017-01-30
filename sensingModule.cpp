#ifndef __SENSING_MODULE_CPP
#define __SENSING_MODULE_CPP

#include<sensingModule.h>
#include<math.h>
#include<stdlib.h>
#ifndef NON_ARDUINO_PROJECT
#include<Arduino.h>
#else
//#include<cmath.h>
#endif


// version : voltageDifference
//long sensingModule::calculateCurrentPosition(int voltageCommand,int voltageSensing)
//{		
//	// voltage command 24V/255incs , 0.0941V/inc
//	// voltage sensing 5V/1024incs , 0.00488V/inc
//
//	//voltageDiffence = (float)(abs(voltageCommand)) * 0.0941f - (float)voltageSensing * 0.00488f;		// had to paramertized later
//	float __voltageCommand = (float)(abs(voltageCommand) * 24 / 255);
//	voltageDiffence = __voltageCommand - ((float)voltageSensing * 5 / 1024);		// had to paramertized later
//	
//	if (voltageDiffence < (__voltageCommand - 1.5f) || voltageDiffence < 0){
//		// stop voltage  = command voltage - drop voltage
//		// out of bound , mean no moving 
//		voltageDiffence = 0;
//	}
//	//todo , may use digital low-pass filter to filter-out high frequency error signal? to increase accuracy
//
//
//	// once the voltage command less than zero , means reverse direction
//	if (voltageCommand < 0){
//		voltageDiffence *= -1;
//	}
//	
//
//	currentPosition += voltageDiffence /** timeInterval*/;
//	currentSpeed = voltageDiffence;
//
//	return currentPosition;
//}



// vesrion : pure voltageSense
//long sensingModule::calculateCurrentPosition(int voltageCommand, int voltageSensing)
//{
//	// voltage command 24V/255incs , 0.0941V/inc
//	// voltage sensing 5V/1024incs , 0.00488V/inc
//
//	if (voltageSensing > 300 || voltageSensing < 70)
//	{
//		// out of bound
//		voltageDiffence = 0;
//	}
//	else
//	{
//		voltageDiffence = 300 - voltageSensing;
//	}
//
//
//	// once the voltage command less than zero , means reverse direction
//	if (voltageCommand < 0){
//		voltageDiffence *= -1;
//	}
//
//	currentPosition += voltageDiffence /** timeInterval*/;
//	currentSpeed = voltageDiffence;
//
//	return currentPosition;
//}

long sensingModule::calculateCurrentPosition(int voltageCommand, int voltageSensing)
{
	// voltage command 24V/255incs , 0.0941V/inc
	// voltage sensing 5V/1024incs , 0.00488V/inc

	//voltageDiffence = (float)(abs(voltageCommand)) * 0.0941f - (float)voltageSensing * 0.00488f;		// had to paramertized later
	float __voltageCommand = (float)(abs(voltageCommand) * 24 / 255);
	float __voltageSense = ((float)voltageSensing * 5 / 1024);
	float __voltageStall = __voltageCommand * 3 / 53;
	float omega = __voltageStall - __voltageSense;

	if (omega < 0)
		omega = 0;

	//todo , may use digital low-pass filter to filter-out high frequency error signal? to increase accuracy


	// once the voltage command less than zero , means reverse direction
	if (voltageCommand < 0){
		omega *= -1;
	}


	currentPosition += omega /** timeInterval*/;
	currentSpeed = omega;

	return currentPosition;
}



sensingModule::sensingModule()
{
	currentPosition = 0;
}

#endif