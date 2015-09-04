/*
     File: Filter.cpp
  Version: 1.0
 
*/
#include "Filter.h"

#pragma mark ____Filter


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Standard DSP AudioUnit implementation

AUDIOCOMPONENT_ENTRY(AUBaseProcessFactory, Filter)




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Construction_Initialization


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::Filter
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Filter::Filter(AudioUnit component) : AUEffectBase(component)
{
	// all the parameters must be set to their initial values here
	//
	// these calls have the effect both of defining the parameters for the first time
	// and assigning their initial values
	//
	SetParameter(kFilterParam_CutoffFrequency, kDefaultCutoff );
	SetParameter(kFilterParam_Resonance, kDefaultResonance );
	SetParameter(kFilterParam_FilterType, kLowpassFilter);
	
	// kFilterParam_CutoffFrequency max value depends on sample-rate
	SetParamHasSampleRateDependency(true );
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::Initialize
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Filter::Initialize()
{
	OSStatus result = AUEffectBase::Initialize();
	
	if(result == noErr )
	{
		// in case the AU was un-initialized and parameters were changed, the view can now
		// be made aware it needs to update the frequency response curve
		PropertyChanged(kAudioUnitCustomProperty_FilterFrequencyResponse, kAudioUnitScope_Global, 0 );
	}
	
	return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Parameters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::GetParameterInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Filter::GetParameterInfo(	AudioUnitScope			inScope,
									AudioUnitParameterID	inParameterID,
									AudioUnitParameterInfo	&outParameterInfo )
{
	OSStatus result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						+		kAudioUnitParameterFlag_IsReadable;
		
	if (inScope == kAudioUnitScope_Global)
	{
		switch(inParameterID)
		{
			case kFilterParam_CutoffFrequency:
				AUBase::FillInParameterName (outParameterInfo, kCutoffFreq_Name, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
				outParameterInfo.minValue = kMinCutoffHz;
				outParameterInfo.maxValue = GetSampleRate() * 0.5;
				outParameterInfo.defaultValue = kDefaultCutoff;
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
				outParameterInfo.flags += kAudioUnitParameterFlag_DisplayLogarithmic;
				break;
				
			case kFilterParam_Resonance:
				AUBase::FillInParameterName (outParameterInfo, kResonance_Name, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
				outParameterInfo.minValue = kMinResonance;
				outParameterInfo.maxValue = kMaxResonance;
				outParameterInfo.defaultValue = kDefaultResonance;
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
				break;
				
			case kFilterParam_FilterType:
				AUBase::FillInParameterName(outParameterInfo, kFilterType_Name, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
				outParameterInfo.minValue = kLowpassFilter;
				outParameterInfo.maxValue = kHighpassFilter;
				outParameterInfo.defaultValue = kLowpassFilter;
				break;
				
			default:
				result = kAudioUnitErr_InvalidParameter;
				break;
		}
	}
	else
	{
		result = kAudioUnitErr_InvalidParameter;
	}
	
	return result;
}

#pragma mark ____GetParameterValueStrings

/*
 *TremoloUnit::GetParameterValueStrings()
 */
ComponentResult Filter::GetParameterValueStrings(AudioUnitScope inScope, AudioUnitParameterID inParameterID,
													  CFArrayRef *outStrings)
{
	if((inScope == kAudioUnitScope_Global) && (inParameterID == kFilterParam_FilterType))
	{
		if (outStrings == NULL)
		{
			return noErr;
		}
		
		CFStringRef strings[] = { kLowpass_Name, kHighpass_Name };
		
		*outStrings = CFArrayCreate(NULL, (const void **) strings, (sizeof(strings) / sizeof(strings[0])), NULL);
		
		return noErr;
	}
	
	return kAudioUnitErr_InvalidParameter;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::GetPropertyInfo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Filter::GetPropertyInfo (	AudioUnitPropertyID				inID,
									AudioUnitScope					inScope,
									AudioUnitElement				inElement,
									UInt32 &						outDataSize,
									Boolean &						outWritable)
{
	if (inScope == kAudioUnitScope_Global)
	{
		switch (inID)
		{
			case kAudioUnitProperty_CocoaUI:
				outWritable = false;
				outDataSize = sizeof (AudioUnitCocoaViewInfo);
				return noErr;
			
			case kAudioUnitCustomProperty_FilterFrequencyResponse:	// our custom property
				if(inScope != kAudioUnitScope_Global ) return kAudioUnitErr_InvalidScope;
				outDataSize = kNumberOfResponseFrequencies * sizeof(FrequencyResponse);
				outWritable = false;
				return noErr;
		}
	}
	
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::GetProperty
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Filter::GetProperty (AudioUnitPropertyID inID,
							  AudioUnitScope inScope,
							  AudioUnitElement inElement,
							  void *outData)
{
/*	if (inScope == kAudioUnitScope_Global)
	{
		switch (inID)
		{
			// This property allows the host application to find the UI associated with this
			// AudioUnit
			//
			case kAudioUnitProperty_CocoaUI:
			{
				// Look for a resource in the main bundle by name and type.
				CFBundleRef bundle = CFBundleGetBundleWithIdentifier( CFSTR("com.DEMO.audiounit.Filter") );
				
				if (bundle == NULL) return fnfErr;
                
				CFURLRef bundleURL = CFBundleCopyResourceURL( bundle, 
                    CFSTR("CocoaFilterView"),	// this is the name of the cocoa bundle as specified in the CocoaViewFactory.plist
                    CFSTR("bundle"),			// this is the extension of the cocoa bundle
                    NULL);
                
                if (bundleURL == NULL) return fnfErr;
                
				CFStringRef className = CFSTR("DEMOFilter_ViewFactory");	// name of the main class that implements the AUCocoaUIBase protocol
				AudioUnitCocoaViewInfo cocoaInfo = { bundleURL, { className } };
				*((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
				
				return noErr;
			}

			// This is our custom property which reports the current frequency response curve
			//
			case kAudioUnitCustomProperty_FilterFrequencyResponse:
			{
				if(inScope != kAudioUnitScope_Global) 	return kAudioUnitErr_InvalidScope;

				// the kernels are only created if we are initialized
				// since we're using the kernels to get the curve info, let
				// the caller know we can't do it if we're un-initialized
				// the UI should check for the error and not draw the curve in this case
				if(!IsInitialized() ) return kAudioUnitErr_Uninitialized;

				FrequencyResponse *freqResponseTable = ((FrequencyResponse*)outData);

				// each of our filter kernel objects (one per channel) will have an identical frequency response
				// so we arbitrarilly use the first one...
				//
				FilterKernel *filterKernel = dynamic_cast<FilterKernel*>(mKernelList[0]);


				double cutoff = GetParameter(kFilterParam_CutoffFrequency);
				double resonance = GetParameter(kFilterParam_Resonance );

				float srate = GetSampleRate();
				
				cutoff = 2.0 * cutoff / srate;
				if(cutoff > 0.99) cutoff = 0.99;		// clip cutoff to highest allowed by sample rate...

				filterKernel->CalculateLopassParams(cutoff, resonance);
				
				for(int i = 0; i < kNumberOfResponseFrequencies; i++ )
				{
					double frequency = freqResponseTable[i].mFrequency;
					
					freqResponseTable[i].mMagnitude = filterKernel->GetFrequencyResponse(frequency);
				}

				return noErr;
			}
		}
	}
*/	
	// if we've gotten this far, handles the standard properties
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____Presets

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::GetPresets
//	const after declaration implies it is a read only function
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Filter::GetPresets (CFArrayRef *outData) const
{
		// this is used to determine if presets are supported 
		// which in this unit they are so we implement this method!
	if (outData == NULL) return noErr;
	
	CFMutableArrayRef theArray = CFArrayCreateMutable (NULL, kNumberPresets, NULL);
	for (int i = 0; i < kNumberPresets; ++i)
	{
		CFArrayAppendValue (theArray, &kPresets[i]);
    }
    
	*outData = (CFArrayRef)theArray;	// client is responsible for releasing the array
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::NewFactoryPresetSet
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Filter::NewFactoryPresetSet (const AUPreset & inNewFactoryPreset)
{
	SInt32 chosenPreset = inNewFactoryPreset.presetNumber;
	
	for(int i = 0; i < kNumberPresets; ++i)
	{
		if(chosenPreset == kPresets[i].presetNumber)
		{
			// set whatever state you need to based on this preset's selection
			//
			// Here we use a switch statement, but it would also be possible to
			// use chosenPreset as an index into an array (if you publish the preset
			// numbers as indices in the GetPresets() method)
			//			
			switch(chosenPreset)
			{
				case kPreset_One:
					SetParameter(kFilterParam_CutoffFrequency, 200.0 );
					SetParameter(kFilterParam_Resonance, -5.0 );
					break;
				case kPreset_Two:
					SetParameter(kFilterParam_CutoffFrequency, 1000.0 );
					SetParameter(kFilterParam_Resonance, 10.0 );
					break;
			}
            
            SetAFactoryPresetAsCurrent (kPresets[i]);
			return noErr;
		}
	}
	
	return kAudioUnitErr_InvalidPropertyValue;
}

