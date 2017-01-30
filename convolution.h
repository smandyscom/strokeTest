#ifndef __CONVOLUTION_H
#define __CONVOLUTION_H

//#define WINDOW_LENGTH 29		// couldnt over 128 ,the sram restriction , Hsien 2015.01.19

#include<stdlib.h>
#include<string.h>

#define BUFFER_TYPE int

class convolution
{

public:

	long convolutionCalculate(BUFFER_TYPE xi);	// queue in input , and export calculated value
	
	convolution(int __length , BUFFER_TYPE* _xt){
		//------------------
		//	The constructor
		//------------------
		length = __length;

		this->writeIndex=0;		// initialize queue

		ht = (BUFFER_TYPE*)malloc(sizeof(BUFFER_TYPE) * length);		// allocating ht
		memset(ht, 0, sizeof(BUFFER_TYPE) *length);						// initial the ht

		xt = _xt;		// link xt

		//----------------------
		//	Normalizing Factor Calculation
		//----------------------
		normalizingFactor = 0;
		for (int i = 0; i<length; i++){
			normalizingFactor += xt[i];
		}
	}
	
protected:
	BUFFER_TYPE* xt;		// the x-tao data
	int normalizingFactor;		//normalizing factor verified , Hsien , 2015.05.03
	int writeIndex;
	int length;

	BUFFER_TYPE *ht/*[WINDOW_LENGTH]*/;		// the h(t-tao) data 
	//---------------------------
	//	Fixed length ring buffer control
	//---------------------------		
	void enqueue(BUFFER_TYPE xi);			// queue xi into ring buffer , wourld override oldest data
	BUFFER_TYPE readIndex(int index);		// read specific index of ring buffer
};

#endif
