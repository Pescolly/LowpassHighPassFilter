//
//  FilterTypes.cpp
//  LowpassHighpassFilter
//
//  Created by armen karamian on 9/4/15.
//
//

#include "FilterTypes.h"



#pragma mark FilterhelperFunctions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterTypes
	//
	//		void FilterKernel::Process(const Float32 *inSourceP, Float32 *inDestP, UInt32 inFramesToProcess,
	//			UInt32 inNumChannels, bool &ioSilence)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

std::vector<Float32> FilterTypes::getFilterCoefficients(Float32 cutoffFrequency, Float32 resonance, Float64 sampleRate, int selectedFilterType)
{
	switch (selectedFilterType)
	{
		case kLowpassFilter:
		{
				//			Float64 linearResonanceInput = pow(10.0, 0.05 * -resonance);		// convert from decibels to linear
				//			Float64 resonance = 1 - M_PI * (100 / sampleRate);
			Float32 k = tan((M_PI * cutoffFrequency) / sampleRate);					//lambda
			Float32 kSquared = pow(k, 2);
			Float32 rootTwo = sqrt(2);
			
			Float32 a0 = kSquared / (1 + rootTwo * k + kSquared);							//calculate coefficents
			Float32 a1 = (2 * kSquared) / (1 + rootTwo * k + kSquared);
			Float32 a2 = kSquared / (1 + rootTwo * k + kSquared);
			Float32 b1 = 2 * (kSquared - 1) / (1 + rootTwo * k + kSquared);
			Float32 b2 = (1 - rootTwo * k + kSquared) / (1 + rootTwo * k + kSquared);

			std::vector<Float32> coefficients(5, 0);											//create vector with coefficients
			coefficients[A0] = a0;
			coefficients[A1] = a1;
			coefficients[A2] = a2;
			coefficients[B1] = b1;
			coefficients[B2] = b2;
			
			return coefficients;
		}
			
		case kHighpassFilter:
		{
			Float64 lambda = ((M_PI * cutoffFrequency) / sampleRate);
			Float64 lambdaSquared = pow(lambda, 2);
			
			Float64 a0 = 1 / (1 + (2 * lambda) + lambdaSquared);							//calculate coefficents
			Float64 a1 = 2 * a0;
			Float64 a2 = a0;
			Float64 b1 = 2 * a0 * (lambdaSquared - 1);
			Float64 b2 = a0 * (1 - (2 * lambda) + lambdaSquared);
			
			std::vector<Float32> coefficients(5, 0);											//create vector with coefficients
			coefficients[A0] = a0;
			coefficients[A1] = a1;
			coefficients[A2] = a2;
			coefficients[B1] = b1;
			coefficients[B2] = b2;
			
			return coefficients;
			
		}
			
/*
		case kResonatorFilter:
			break;
			
		case kBandpassFilter:
			break;
*/
		default:
			std::vector<Float32> empty;
			return empty;
	}
}


#pragma mark FilterTypeFunctions
	/*
	 * in progress
	 * LP, HP, BP use the following formula: 			//y[n] = a0x[n] + a1x[n-1] + a2x[n-2] - b1y[n-1] - b2y[n-2]
	 * resonant filter uses:							//y[n] = x[n] - a2x[n] - b1y[n-1] - b2y[n-2]
	 *
	 */

void FilterTypes::lowpassFilter(const Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, std::vector<Float32> coefficients)
{
	for (int n = 2; n < bufferSize; n++)			//start at index n=2 to avoid segfault
	{
		outputBuffer[n] = (coefficients[A0] * inputBuffer[n])
						+ (coefficients[A1] * inputBuffer[n-1])
						+ (coefficients[A2] * inputBuffer[n-2])
						- (coefficients[B1] * outputBuffer[n-1])
						- (coefficients[B2] * outputBuffer[n-2]);
	}
}

void FilterTypes::highpassFilter(const Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, std::vector<Float32> coefficients)
{
	for (int n = 2; n < bufferSize; n++)			//start at index n=2 to avoid segfault
	{
		outputBuffer[n] = (coefficients[A0] * inputBuffer[n])
						+ (coefficients[A1] * inputBuffer[n-1])
						+ (coefficients[A2] * inputBuffer[n-2])
						- (coefficients[B1] * outputBuffer[n-1])
						- (coefficients[B2] * outputBuffer[n-2]);
	}

 }
 
/*
float FilterTypes::bandpassFilter(float *inputBuffer, float resonantFrequency, float bandwidth, int bufferSize, float sampleRate)
{
	double filterDiameter, filterRadius, filterRadiusSquared, cos_theta, scale, width;
	filterRadius = 1.0 - M_PI * (bandwidth / sampleRate);
	filterDiameter = 2 * (filterRadius);
	filterRadiusSquared = pow(filterRadius, 2);
	cos_theta = (filterDiameter / (1.0 + filterRadiusSquared)) * cos(2 * M_PI * resonantFrequency / sampleRate);
	scale = 1 - filterRadius;
	float delay[2] = {0.f, 0.f};
	
	for (int i = 0; i < bufferSize; i++)
	{
		width = scale * inputBuffer[i] + filterDiameter * cos_theta * delay[0] - filterRadiusSquared * delay[1];
		inputBuffer[i] = (float)(width - filterRadius * delay[1]);
		delay[1] = delay[0];
		delay[0] = (float) width;
	}
	
	return *inputBuffer;
}

float FilterTypes::resonator(float *inputBuffer, float resonantFrequency, float bandwidth, int bufferSize, float sampleRate)
{
	double filterDiameter, filterRadius, filterRadiusSquared, cos_theta, scale, width;
	
	filterRadius = 1.0 - M_PI * (bandwidth / sampleRate);
	filterDiameter = 2 * filterRadius;
	filterRadiusSquared = pow(filterRadius, 2);
	cos_theta = (filterDiameter / (1.0 + filterRadiusSquared)) * cos(2 * M_PI * resonantFrequency / sampleRate);
	scale = 1 - filterRadius;
	float delay[2] = {0.f, 0.f};
	
	for (int i = 0; i < bufferSize; i++)
	{
		width = scale * inputBuffer[i] + filterDiameter * cos_theta * delay[0] - filterRadiusSquared * delay[1];
		inputBuffer[i] = (float) (width - filterRadius * delay[1]);
		delay[1] = delay[0];
		delay[0] = (float) width;
	}
	return *inputBuffer;
}
 */
