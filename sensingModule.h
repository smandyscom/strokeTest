#ifndef __SENSING_MODULE_H
#define __SENSING_MODULE_H

class sensingModule
{
public:
	//int* voltageCommand;		// the command voltage established on motor end
	//int* voltageSensing;		// the reference voltage sensed from motor end
	//int timeInterval;

	float getCurrentPosition(){ return currentPosition; };
	float getCurrentSpeed(){ return currentSpeed; };
	void clearCurrentPosition(){ currentPosition = 0; };

	long calculateCurrentPosition(int voltageCommand, int voltageSensing);		// calculating current position

	sensingModule();

private:
	float currentPosition;		//32bits signed integer in Arduino
	float currentSpeed;	// the voltage difference
	float voltageDiffence;
};


#endif