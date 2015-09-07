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



class FilterTypes
{
public:
	void lowpassFilter(const Float32 *inputBuffer, Float32 *outputBuffer, float freq, int bufferSize, float sampleRate);
	float highpassFilter(float *inputBuffer, float freq, int bufferSize, float sampleRate);
	float bandpassFilter(float *inputBuffer, float freq, float bandwidth, int bufferSize, float sampleRate);
	float resonator(float *inputBuffer, float freq, float bandwidth, int bufferSize, float sampleRate);
	
};

#endif /* defined(__LowpassHighpassFilter__FilterTypes__) */
