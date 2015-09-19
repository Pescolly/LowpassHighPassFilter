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
#include "FilterParameters.h"
#include <vector>

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

		// used by the custom property handled in the Filter class below
	double GetFrequencyResponse( double inFreq );
	
	
	
private:
	
	double mLastCutoff = 0;
	std::vector<Float32> coefficients;
	double	mLastResonance = 0;
};

#endif /* defined(__LowpassHighpassFilter__FilterKernel__) */
