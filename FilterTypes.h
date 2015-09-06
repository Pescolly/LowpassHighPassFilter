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
	float lowpassFilter(float *inputBuffer, float freq, float *delay, int bufferSize, float sampleRate);
	float highpassFilter(float *inputBuffer, float freq, float *delay, int bufferSize, float sampleRate);
	float bandpassFilter(float *inputBuffer, float freq, float bandwidth, float *delay, int bufferSize, float sampleRate);
	float resonator(float *inputBuffer, float freq, float bandwidth, float *delay, int bufferSize, float sampleRate);
	
};

#endif /* defined(__LowpassHighpassFilter__FilterTypes__) */
