#ifndef CONVOLUTION_CPP
#define CONVOLUTION_CPP

#include <convolution.h>
//#include "math.h"
//#include "stdio.h"

long convolution::convolutionCalculate(BUFFER_TYPE xi){
	long outputValue = 0;
	this->enqueue(xi);
	for(int i=0;i<length;i++){
		outputValue += xt[i] * readIndex(i);
	}
	//return outputValue;
	return outputValue / normalizingFactor;
}

void convolution::enqueue(BUFFER_TYPE xi){
	ht[writeIndex] = xi;
	writeIndex++;					// increment the writeIndex
	writeIndex %= length;	// overflow - rewind
}

BUFFER_TYPE convolution::readIndex(int index){
	return ht[(writeIndex + index)%length];
}

#endif