//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
// This script was adapted and referenced from Reiss and McPherson (2015) and Tarr (2019).
// Please refer to accompanying report reference list for full reference details.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace delayEffectProcessor {

//------------------------------------------------------------------------
//  delay2Processor
//------------------------------------------------------------------------
class delay2Processor : public Steinberg::Vst::AudioEffect
{
public:
	delay2Processor ();
	~delay2Processor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new delay2Processor; 
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
    // These values are used for the processing.
    float m_gain;
    float m_delayLength;
    float m_dryMix;
    float m_wetMix;
    float m_feedback;
    
private:
    std::vector<std::vector<float>> m_delayBuffer;
    int m_delayBufferLength;
    int m_delayReadPosition;
    int m_delayWritePosition;
    double m_sampleRate;
    
    void resizeDelayBuffer();
    float convertGainFromNormalized(Steinberg::Vst::ParamValue mGain);
    float convertDelayLengthFromNormalized(Steinberg::Vst::ParamValue mDelayLength);
    float convertDryMixFromNormalized(Steinberg::Vst::ParamValue mDryMix);
    float convertWetMixFromNormalized(Steinberg::Vst::ParamValue mWetMix);
    float convertFeedbackFromNormalized(Steinberg::Vst::ParamValue mFeedback);
};

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
