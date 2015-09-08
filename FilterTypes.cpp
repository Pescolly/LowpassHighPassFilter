//
//  FilterTypes.cpp
//  LowpassHighpassFilter
//
//  Created by armen karamian on 9/4/15.
//
//

#include "FilterTypes.h"


using namespace std;

#pragma mark FilterhelperFunctions
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	FilterTypes
	//
	//		void FilterKernel::Process(const Float32 *inSourceP, Float32 *inDestP, UInt32 inFramesToProcess,
	//			UInt32 inNumChannels, bool &ioSilence)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

vector<double> FilterTypes::getFilterCoefficients(float resonantFrequency, float resonance, float sampleRate, int selectedFilterType)
{
	switch (selectedFilterType)
	{
		case kLowpassFilter:
		{
			double lambda = 1 / tan((M_PI * resonantFrequency) / sampleRate);
			double lambdaSquared = pow(lambda, 2);
			
			double a0 = 1 / ((1 + (2 * lambda)) + lambdaSquared);							//calculate coefficents
			double a1 = 2 * a0;
			double a2 = a0;
			double b1 = 2 * a0 * (1 - lambdaSquared);
			double b2 = a0 * (1 - (2 * lambda) + lambdaSquared);

			vector<double> coefficients(5, 0);											//create vector with coefficients
			coefficients[A0] = a0;
			coefficients[A1] = a1;
			coefficients[A2] = a2;
			coefficients[B1] = b1;
			coefficients[B2] = b2;
			
			return coefficients;
		}
			
		case kHighpassFilter:
			break;
		
		case kResonatorFilter:
			break;
			
		case kBandpassFilter:
			break;

		default:
			vector<double> empty;
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

void FilterTypes::lowpassFilter(const Float32 *inputBuffer, Float32 *outputBuffer, int bufferSize, vector<double> coefficients)
{
	for (int n = 2; n < bufferSize; n++)			//start at index n=2 to avoid segfault
	{
		outputBuffer[n] = (coefficients[A0] * inputBuffer[n]) + (coefficients[A1] * inputBuffer[n-1]) + (coefficients[A2] * inputBuffer[n-2])
		- (coefficients[B1] * outputBuffer[n-1]) - (coefficients[B2] * outputBuffer[n-2]);		
	}
}

	//incomplete below...
float FilterTypes::highpassFilter(float *inputBuffer, float resonantFrequency,  float resonance, int bufferSize, float sampleRate)
{
	double cos_theta, coEfficient;
	cos_theta = 2.0 - cos(2 * M_PI * resonantFrequency / sampleRate);
	coEfficient = cos_theta = sqrt(pow(cos_theta,2) - 1.0);
	float delay[2] = {0.f, 0.f};
	
	for (int i = 0; i < bufferSize; i++)
	{
		inputBuffer[i] = (float) (inputBuffer[i] * (1 - coEfficient) - *delay * coEfficient);
		*delay = inputBuffer[i];
	}
	return *inputBuffer;
}

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
