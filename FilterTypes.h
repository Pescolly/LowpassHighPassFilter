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

#define A0 0
#define A1 1
#define A2 2
#define B1 3
#define B2 4

using namespace std;

class FilterTypes
{
public:
	/*
	 *	Filter types
	 *	Algorithms derived from the formula give in The Audio Programming Book, pg. 484
	 */
	void lowpassFilter(const Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, vector<double> coefficients);
	float highpassFilter(float *inputBuffer, float resonantFrequency, float resonance, int bufferSize, float sampleRate);
	float bandpassFilter(float *inputBuffer, float resonantFrequency, float bandwidth, int bufferSize, float sampleRate);
	float resonator(float *inputBuffer, float resonantFrequency, float bandwidth, int bufferSize, float sampleRate);
	
	/*
	 *	Filter helper functions
	 */
	
		//get from second-order butterworth coefficients table on pg. 484
	vector<double> getFilterCoefficients(float resonantFrequency, float resonance, float sampleRate, int selectedFilterType);
	
};

#endif /* defined(__LowpassHighpassFilter__FilterTypes__) */
