/*
     File: Filter.h
 Abstract: Filter.h
  Version: 1.0
 
*/


#include <math.h>
#include "AUEffectBase.h"
#include <AudioToolbox/AudioUnitUtilities.h>
#include "FilterVersion.h"
#include "FilterKernel.h"
#include "FilterParameters.h"

class Filter : public AUEffectBase
{
public:
	Filter(AudioUnit component);
	
	virtual OSStatus			Version() { return kFilterVersion; }
	
	virtual OSStatus			Initialize();
	
	virtual AUKernelBase *		NewKernel() { return new FilterKernel(this); }
	
		// for custom property
	virtual OSStatus			GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &				outDataSize,
												Boolean	&				outWritable );
	
	virtual OSStatus			GetProperty(AudioUnitPropertyID 	inID,
											AudioUnitScope 			inScope,
											AudioUnitElement 		inElement,
											void 					* outData );
	
	ComponentResult 			GetParameterValueStrings(AudioUnitScope inScope,
														 AudioUnitParameterID inParameterID,
														 CFArrayRef *outStrings);

	
	virtual OSStatus			GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo );
	
		// handle presets:
	virtual OSStatus			GetPresets(CFArrayRef	*outData)	const;
	virtual OSStatus			NewFactoryPresetSet (const AUPreset &inNewFactoryPreset);
	
		// we'll report a 1ms tail.   A reverb effect would have a much more substantial tail on
		// the order of several seconds....
		//
	virtual	bool				SupportsTail () { return true; }
	virtual Float64				GetTailTime() {return 0.001;}
	
		// we have no latency
		//
		// A lookahead compressor or FFT-based processor should report the true latency in seconds
	virtual Float64				GetLatency() {return 0.0;}

	
protected:
		// Here we define a custom property so the view is able to retrieve the current frequency
		// response curve.  The curve changes as the filter's cutoff frequency and resonance are
		// changed...

		// custom properties id's must be 64000 or greater
		// see <AudioUnit/AudioUnitProperties.h> for a list of Apple-defined standard properties
		//
	enum
	{
		kAudioUnitCustomProperty_FilterType = 65537
	};

		// We'll define our property data to be a size kNumberOfResponseFrequencies array of structs
		// The UI will pass in the desired frequency in the mFrequency field, and the Filter AU
		// will provide the linear magnitude response of the filter in the mMagnitude field
		// for each element in the array.
};

const float kMinCutoffHz = 20.0;
const float kDefaultCutoff = 1000.0;
const float kMaxCutoff = 20000.0;
const float kMinResonance = -20.0;
const float kMaxResonance = 20.0;
const float kDefaultResonance = 0;




	// Factory presets
static const int kPreset_One = 0;
static const int kPreset_Two = 1;
static const int kNumberPresets = 2;

static AUPreset kPresets[kNumberPresets] =
{
	{ kPreset_One, CFSTR("Preset One") },
	{ kPreset_Two, CFSTR("Preset Two") }
};

static const int kPresetDefault = kPreset_One;
static const int kPresetDefaultIndex = 0;