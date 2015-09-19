//
//  FilterKernel.cpp
//  LowpassHighpassFilter
//
//  Created by armen karamian on 8/28/15.
//
//

#include "FilterKernel.h"

using namespace std;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____FilterKernel


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::FilterKernel()
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FilterKernel::FilterKernel(AUEffectBase *inAudioUnit) : AUKernelBase(inAudioUnit)
{
	Reset();
}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::~FilterKernel()
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FilterKernel::~FilterKernel( )
{
}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::Reset()
	//
	//		It's very important to fully reset all filter state variables to their
	//		initial settings here.  For delay/reverb effects, the delay buffers must
	//		also be cleared here.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FilterKernel::Reset()
{
	coefficients.assign(5, 0);
	coefficients[A0] = 1;
	coefficients[A1] = .07;
	coefficients[A2] = .14;
	coefficients[B1] = -1.14;
	coefficients[B2] = .412;
}



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::Process(int inFramesToProcess)
	//
	//		We process one non-interleaved stream at a time
	// for version 2 AudioUnits inNumChannels is always 1
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FilterKernel::Process(const Float32 *inSourceP, Float32 *inDestP, UInt32 inFramesToProcess,
						   UInt32 inNumChannels, bool &ioSilence)
{
	if (!ioSilence)
	{
			//create filter selection and filter function objects
		int selectedFilterType = (int) GetParameter(kFilterParam_FilterType);
		Float32 cutoffFrequency = GetParameter(kFilterParam_ResonantFrequency);
		mLastCutoff = cutoffFrequency;
		Float32 resonance = GetParameter(kFilterParam_Resonance);
		Float64 sampleRate = GetSampleRate();
		const Float32 *inputBuffer = inSourceP;
		
		FilterTypes filters;
		
			//determine what filter is selected and pass buffer and arguments in
		switch (selectedFilterType)
		{
			case kLowpassFilter:
			{
/*
				if (mLastCutoff != cutoffFrequency)			//recalculate coeff. if old and new cutoff don't match
				{
					coefficients = filters.getFilterCoefficients(cutoffFrequency, resonance, sampleRate, selectedFilterType); //get coefficients
				}																										  //}
*/
 				filters.lowpassFilter(inputBuffer, inDestP, inFramesToProcess, coefficients); //call LPF
				break;
			}
				
			case kHighpassFilter:
			{
				coefficients = filters.getFilterCoefficients(cutoffFrequency, resonance, sampleRate, selectedFilterType); //get coefficients
				filters.highpassFilter(inputBuffer, inDestP, inFramesToProcess, coefficients); //call HPF
				break;
			}
				
			case kBandpassFilter:
				
				break;
			case kResonatorFilter:
				break;
				
  			default:
				break;
		}
	}
}
