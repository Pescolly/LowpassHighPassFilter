//
//  FilterKernel.h
//  LowpassHighpassFilter
//
//  Created by armen karamian on 8/28/15.
//
//

#ifndef __LowpassHighpassFilter__FilterKernel__
#define __LowpassHighpassFilter__FilterKernel__

#include <stdio.h>
#include <math.h>
#include "AUEffectBase.h"
#include <AudioToolbox/AudioUnitUtilities.h>
#include "FilterVersion.h"
#include "FilterTypes.h"

#pragma mark FilterKernel

class FilterKernel : public AUKernelBase		// the actual filter DSP happens here
{
public:
	FilterKernel(AUEffectBase *inAudioUnit);
	virtual ~FilterKernel();
	
		// processes one channel of non-interleaved samples
	virtual void Process(const  Float32 *inSourceP,
						 		Float32 *inDestP,
								UInt32 inFramesToProcess,
						 		UInt32 inNumChannels,
						 		bool &ioSilence);
	
		// resets the filter state
	virtual void Reset();
	
		//set filter type
	void setFilterType(AudioUnitParameterValue inputParameterValue);
	
	void CalculateLopassParams(	double inFreq, double inResonance );
	
		// used by the custom property handled in the Filter class below
	double GetFrequencyResponse( double inFreq );
	
	
	
private:
	
	AudioUnitParameterValue parameterValue;
	
	
		//high/low pass select
	bool lowpassON;
	
		// filter coefficients
	double	mA0;
	double	mA1;
	double	mA2;
	double	mB1;
	double	mB2;
	
		// filter state
	double	mX1;
	double	mX2;
	double	mY1;
	double	mY2;
	
	double	mLastCutoff;
	double	mLastResonance;
};

#endif /* defined(__LowpassHighpassFilter__FilterKernel__) */
