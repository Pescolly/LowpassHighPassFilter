//
//  FilterTypes.h
//  LowpassHighpassFilter
//
//  Created by armen karamian on 9/4/15.
//
//

#ifndef __LowpassHighpassFilter__FilterTypes__
#define __LowpassHighpassFilter__FilterTypes__

#include <AudioToolbox/AudioToolbox.h>
#include "FilterParameters.h"
#include <vector>

const int A0 = 0;
const int A1 = 1;
const int A2 = 2;
const int B1 = 3;
const int B2 = 4;

class FilterTypes
{
public:
	/*
	 *	Filter types
	 *	Algorithms derived from the formula give in The Audio Programming Book, pg. 484
	 */
	void lowpassFilter(const Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, std::vector<Float32> coefficients);
	float highpassFilter(Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, std::vector<Float32> coefficients);
	float bandpassFilter(Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, std::vector<Float32> coefficients);
	float resonator(Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, std::vector<Float32> coefficients);
	
	
	
	/*
	 *	Filter helper functions
	 */
	
		//get from second-order butterworth coefficients table on pg. 484
	std::vector<Float32> getFilterCoefficients(Float32 resonantFrequency, Float32 resonance, Float64 sampleRate, int selectedFilterType);
	
};

#endif /* defined(__LowpassHighpassFilter__FilterTypes__) */
