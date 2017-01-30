#ifndef __CWCCWCONTROL
#define __CWCCWCONTROL

#ifdef ARDUINO_PROJECT
#include<Arduino.h>
#endif
class dcMotorOutputBase
{
public:
	virtual int drive(int step) = 0;	// drive motor a step , incremental unit
private:

};

class cwccwControl : public dcMotorOutputBase
{
	// for L298N , 6203
public:
	//cwccwControl();
	//~cwccwControl();
	int drive(int step);		// drive motor a step , unit in MAX_VOLT/255
	void initialize(unsigned char forwardPin,unsigned char backwardPin);
//private:
	unsigned char forwardOutputPin;	// as output
	unsigned char backwardOuputPin;	// as output
	//uint8_t senseInputPin;			// the current sense input pin
	//int senseThreshold;
	//bool(*externalCondition)();				// the function pointer to reject motor to drive
};

class pwmDirection : public dcMotorOutputBase
{
	// for NI-18201
public:
	int drive(int step);		// drive motor a step , unit in MAX_VOLT/255
	void initialize(unsigned char pwmPin, unsigned char directionPin);
//private:
	unsigned char __pwmPin;	// as pwm-output	(pwm duty wave)
	unsigned char __directionPin;	// as direction-output (high/low level)
};




#endif