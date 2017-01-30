#ifndef __DEVICE_H
#define __DEVICE_H
//------------------------------------------
//	The base class for state-control devices
//------------------------------------------
struct deviceBase
{
	bool __enabled;

	unsigned short state;
	void(*Enable)(deviceBase*, bool);

	int(*process)(deviceBase* device);
};

int deviceRunning(deviceBase* device);
void deviceEnable(deviceBase* device, bool __enabled);
int deviceInitialize(deviceBase* device);

#ifdef UNDER_ARDUINO_PROJECT
#include"device.cpp"	// in arudino project , could not pre-build .obj to link , so that should include all cpp at once
#endif

#endif // !__DEVICE
