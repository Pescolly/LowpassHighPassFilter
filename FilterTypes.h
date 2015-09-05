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

enum AudioUnitFilterType
{
	kHighpassFilterType = 909,
	kLowpassFilterType = 606,
	kResonantFilterType = 303,
	kBandpassFilterType = 505
};

class FilterTypes
{
public:
	float lowpassFilter(float *sig, float freq, float *del, int vecsize, float sampleRate);
	float highpassFilter(float *sig, float freq, float *del, int vecsize, float sampleRate);
	float bandpassFilter(float *sig, float freq, float bandwidth, float *del, int vecsize, float sampleRate);
	float resonator(float *sig, float freq, float bandwidth, float *del, int vecsize, float sampleRate);
	
};

#endif /* defined(__LowpassHighpassFilter__FilterTypes__) */
