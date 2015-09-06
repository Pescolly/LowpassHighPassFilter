//
//  FilterParameters.h
//  LowpassHighpassFilter
//
//  Created by armen karamian on 9/6/15.
//
//

#ifndef LowpassHighpassFilter_FilterParameters_h
#define LowpassHighpassFilter_FilterParameters_h

	//CFString names for the UI
static CFStringRef kCutoffFreq_Name = CFSTR("Cutoff Frequency");
static CFStringRef kResonance_Name = CFSTR("Resonance");
static CFStringRef kFilterType_Name = CFSTR("Filter Type");
static CFStringRef kLowpass_Name = CFSTR("Lowpass");
static CFStringRef kHighpass_Name = CFSTR("Highpass");
static CFStringRef kBandpass_Name = CFSTR("Bandpass");
static CFStringRef kResonator_Name = CFSTR("Resonator");

/*
	//filter type constants
const int kLowpassFilter = 1;
const int kHighpassFilter = 2;
const int kResonatorFilter = 3;
const int kBandpassFilter = 4;
*/

	//enum of Filter parameters
enum
{
	kFilterParam_CutoffFrequency = 0,
	kFilterParam_Resonance = 1,
	kFilterParam_FilterType = 2,
	kLowpassFilter = 303,
	kHighpassFilter = 505,
	kResonatorFilter = 606,
	kBandpassFilter = 909
};


#endif