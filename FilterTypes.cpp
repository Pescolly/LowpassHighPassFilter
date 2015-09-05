//
//  FilterTypes.cpp
//  LowpassHighpassFilter
//
//  Created by armen karamian on 9/4/15.
//
//

#include "FilterTypes.h"

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterTypes
	//
	//		void FilterKernel::Process(const Float32 *inSourceP, Float32 *inDestP, UInt32 inFramesToProcess,
	//			UInt32 inNumChannels, bool &ioSilence)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//TODO: Move delay to local variable?

float FilterTypes::lowpassFilter(float *inputBuffer, float freq, float *delay, int bufferSize, float sampleRate)
{
	double cos_theta, coEfficient;
	cos_theta = 2.0 - cos(2 * M_PI * freq / sampleRate);
	coEfficient = sqrt(pow(cos_theta,2) - 1.0) - cos_theta;
	
	for (int i = 0; i < bufferSize; i++)
	{
		inputBuffer[i] = (float) (inputBuffer[i] * (1 + coEfficient) - *delay * coEfficient);
		*delay = inputBuffer[i];
	}
	
	return *inputBuffer;
}

float FilterTypes::highpassFilter(float *inputBuffer, float freq, float *delay, int bufferSize, float sampleRate)
{
	double cos_theta, coEfficient;
	cos_theta = 2.0 - cos(2 * M_PI * freq / sampleRate);
	coEfficient = cos_theta = sqrt(pow(cos_theta,2) - 1.0);
	
	for (int i = 0; i < bufferSize; i++)
	{
		inputBuffer[i] = (float) (inputBuffer[i] * (1 - coEfficient) - *delay * coEfficient);
		*delay = inputBuffer[i];
	}
	
	return *inputBuffer;
}

float FilterTypes::bandpassFilter(float *inputBuffer, float freq, float bandwidth, float *delay, int bufferSize, float sampleRate)
{
	double filterDiameter, filterRadius, filterRadiusSquared, cos_theta, scale, width;
	filterRadius = 1.0 - M_PI * (bandwidth / sampleRate);
	filterDiameter = 2 * (filterRadius);
	filterRadiusSquared = pow(filterRadius, 2);
	cos_theta = (filterDiameter / (1.0 + filterRadiusSquared)) * cos(2 * M_PI * freq / sampleRate);
	scale = 1 - filterRadius;
	
	for (int i = 0; i < bufferSize; i++)
	{
		width = scale * inputBuffer[i] + filterDiameter * cos_theta * delay[0] - filterRadiusSquared * delay[1];
		inputBuffer[i] = (float)(width - filterRadius * delay[1]);
		delay[1] = delay[0];
		delay[0] = (float) width;
	}
	
	return *inputBuffer;
}

float FilterTypes::resonator(float *inputBuffer, float freq, float bandwidth, float *delay, int bufferSize, float sampleRate)
{
	
}
