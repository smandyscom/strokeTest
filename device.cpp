#ifndef DEVICE_cpp
#define DEVICE_cpp

#include "device.h"

int deviceRunning(deviceBase* device){
	if (device->__enabled)
		return device->process(device);
	return 0;
}
void deviceEnable(deviceBase* device, bool __enabled){
	((deviceBase*)device)->__enabled = __enabled;
}
int deviceInitialize(deviceBase* device){
	device->state = 0;
	device->__enabled = false;
	device->Enable = deviceEnable;
	//device->process = nullptr;
	return 0;
}

#endif
