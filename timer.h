#ifndef __TIMER_DEVICE_H
#define __TIMER_DEVICE_H
#include"device.h"
//
struct timerDevice
{
	deviceBase base;
	//----------------------------
	//	Propertys
	//----------------------------
	unsigned int TimeInterval;	// in ms
	unsigned int Goal;			// property
	//-----------------------------
	//	Privates
	//-----------------------------
	unsigned int count;
};

int timerDeviceInitialize(timerDevice* __timerDevice);
bool timerDeviceIsTicked(timerDevice* __timerDevice);

#ifdef UNDER_ARDUINO_PROJECT
#include"timer.cpp"	// in arudino project , could not pre-build .obj to link , so that should include all cpp at once
#endif


#endif // !__timerDevice
