
#ifndef __CWCCW_CONTROL_CPP
#define __CWCCW_CONTROL_CPP

#include <cwccwControl.h>

#ifndef NON_ARDUINO_PROJECT
#include <Arduino.h>
#endif



int cwccwControl::drive(int value){

	// no need to know sense info , this object dedicated on cw/ccw control
	//if (!externalCondition() || 
	//	analogRead(senseInputPin) > senseThreshold){
	//	//----------------
	//	//	First Priority : conditon must passed
	//	//----------------
	//	/*analogWrite(this->forwardOutputPin, 0);
	//	analogWrite(this->backwardOuputPin, 0);*/

	//	//todo , should report overcurrent to host control , Hsien 2015.01.23
	//	return 0;
	//}

#ifndef NON_ARDUINO_PROJECT
	long constrainedValue = constrain(round(value),-255,255);
	if (value > 0){
		analogWrite(this->forwardOutputPin, value);
		analogWrite(this->backwardOuputPin, 0);
	}
	else{
		analogWrite(this->forwardOutputPin, 0);
		analogWrite(this->backwardOuputPin, abs(constrainedValue));
	}

#else
	//--------------------------
	//	Console ouput only
	//--------------------------
#endif
	return 0;
}

void cwccwControl::initialize(unsigned char fwdPin
	, unsigned char rwdPin
	/*, uint8_t sensePin
	, int thresHold
	, bool(*stopCondition)()*/)
{
	this->forwardOutputPin = fwdPin;
	this->backwardOuputPin = rwdPin;
	//this->externalCondition = stopCondition;

	//this->senseInputPin = sensePin;
	//this->senseThreshold = thresHold;

	//------------------
	//	Output initiate
	//------------------
#ifndef NON_ARDUINO_PROJECT
	pinMode(this->forwardOutputPin, OUTPUT);
	pinMode(this->backwardOuputPin, OUTPUT);
#endif
	//pinMode(this->senseInputPin, INPUT);

	//analogReference(DEFAULT);	// represent 0-5V in 0-1023
}

int pwmDirection::drive(int value)
{
#ifndef NON_ARDUINO_PROJECT
	long constrainedValue = constrain(round(value), -255, 255);
	if (value > 0){
		analogWrite(this->__pwmPin, value);
		digitalWrite(this->__directionPin, HIGH);
	}
	else{
		analogWrite(this->__pwmPin, value);
		digitalWrite(this->__directionPin, LOW);
	}

#else
	//--------------------------
	//	Console ouput only
	//--------------------------
#endif
	return 0;
}

void pwmDirection::initialize(unsigned char pwmPin, unsigned char directionPin)
{
	this->__directionPin = directionPin;
	this->__pwmPin = pwmPin;
	//------------------
	//	Output initiate
	//------------------
#ifndef NON_ARDUINO_PROJECT
	pinMode(this->__directionPin, OUTPUT);
	pinMode(this->__pwmPin, OUTPUT);
#endif
}


#endif