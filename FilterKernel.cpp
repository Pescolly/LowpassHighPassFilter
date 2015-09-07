//
//  FilterKernel.cpp
//  LowpassHighpassFilter
//
//  Created by armen karamian on 8/28/15.
//
//

#include "FilterKernel.h"

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
	mX1 = 0.0;
	mX2 = 0.0;
	mY1 = 0.0;
	mY2 = 0.0;
	
		// forces filter coefficient calculation
	mLastCutoff = -1.0;
	mLastResonance = -1.0;
}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::CalculateLopassParams()
	//
	//		inFreq is normalized frequency 0 -> 1
	//		inResonance is in decibels
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FilterKernel::CalculateLopassParams(double inFreq, double inResonance )
{
	double r = pow(10.0, 0.05 * -inResonance);		// convert from decibels to linear
	double k = 0.5 * r * sin(M_PI * inFreq);
	double c1 = 0.5 * (1.0 - k) / (1.0 + k);
	double c2 = (0.5 + c1) * cos(M_PI * inFreq);
	double c3 = (0.5 + c1 - c2) * 0.25;
	
	mA0 = 2.0 *   c3;
	mA1 = 2.0 *   2.0 * c3;
	mA2 = 2.0 *   c3;
	mB1 = 2.0 *   -c2;
	mB2 = 2.0 *   c1;
}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::GetFrequencyResponse()
	//
	//		returns scalar magnitude response
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double FilterKernel::GetFrequencyResponse( double inFreq /* in Hertz */ )
{
	float srate = GetSampleRate();
	
	double scaledFrequency = 2.0 * inFreq / srate;
	
		// frequency on unit circle in z-plane
	double zr = cos(M_PI * scaledFrequency);
	double zi = sin(M_PI * scaledFrequency);
	
		// zeros response
	double num_r = mA0*(zr*zr - zi*zi) + mA1*zr + mA2;
	double num_i = 2.0*mA0*zr*zi + mA1*zi;
	
	double num_mag = sqrt(num_r*num_r + num_i*num_i);
	
		// poles response
	double den_r = zr*zr - zi*zi + mB1*zr + mB2;
	double den_i = 2.0*zr*zi + mB1*zi;
	
	double den_mag = sqrt(den_r*den_r + den_i*den_i);
	
		// total response
	double response = num_mag  / den_mag;
	
	
	return response;
}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterKernel::setFilterType(AudioUnitParameterValue inputParameterValue)
	//
	//		set filter type
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void FilterKernel::setFilterType(AudioUnitParameterValue inputParameterValue)
{
	parameterValue = inputParameterValue;
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
		int cutoffFrequency = GetParameter(kFilterParam_CutoffFrequency);
		int resonance = GetParameter(kFilterParam_Resonance);
		float sampleRate = GetSampleRate();
		const Float32 *inputBuffer = inSourceP;
		
		FilterTypes filters;
		
			//determine what filter is selected and pass buffer and arguments in
		switch (selectedFilterType)
		{
			case kLowpassFilter:
			{
				filters.lowpassFilter(inputBuffer, inDestP, cutoffFrequency, inFramesToProcess, sampleRate);
				break;
			}
			case kHighpassFilter:
					//		filters.highpassFilter(<#float *sig#>, <#float freq#>, <#float *del#>, <#int vecsize#>, <#float sampleRate#>)
				break;
			case kBandpassFilter:
					///			filters.bandpassFilter(<#float *sig#>, <#float freq#>, <#float bandwidth#>, <#float *del#>, <#int vecsize#>, <#float sampleRate#>)
				break;
			case kResonatorFilter:
					//			filters.resonator(<#float *sig#>, <#float freq#>, <#float bandwidth#>, <#float *del#>, <#int vecsize#>, <#float sampleRate#>)
				break;
				
  			default:
				break;
		}
	}
}
