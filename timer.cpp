#ifndef __TIMER_CPP
#define __TIMER_CPP

#include "timer.h"

static int timerDeviceProcess(timerDevice* __timerDevice){
	if (__timerDevice->count == 32767)
		__timerDevice->count = 0;	//overflow prevent

	__timerDevice->count++;
	return 0;
}

void timerDeviceEnable(timerDevice* __timerDevice, bool isEnable){
	//if (isEnable != timerDevice->base.Enabled)
	//whether enable or not , count would be reset

	__timerDevice->count = 0;		// timerDevice reset
	__timerDevice->base.__enabled = isEnable;
}

int timerDeviceInitialize(timerDevice* __timerDevice){
	deviceInitialize((deviceBase*)__timerDevice);
	//__timerDevice->base.process = NULL;
	__timerDevice->count = 0;		// timerDevice reset


	//__timerDevice->base.Enable = (void(*)(deviceBase*,bool))timerDeviceEnable;
	((deviceBase*)__timerDevice)->process = (int(*)(deviceBase*))timerDeviceProcess;
	((deviceBase*)__timerDevice)->Enable = (void(*)(deviceBase*, bool))timerDeviceEnable;

	//__timerDevice->base.process = (int(*)(deviceBase*))timerDeviceProcess;
	//__timerDevice->base.Enable = (void(*)(deviceBase*, bool))timerDeviceEnable;
	return 0;
}

// as Readonly property
bool timerDeviceIsTicked(timerDevice* __timerDevice){
	return __timerDevice->count >= __timerDevice->Goal;
}


#endif