//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
// This script was adapted and referenced from Reiss and McPherson (2015) and Tarr (2019).
// Please refer to accompanying report reference list for full reference details.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "circularBuffer.hpp"

namespace delayEffectProcessor {

//------------------------------------------------------------------------
//  delay2Processor
//------------------------------------------------------------------------
class delay2Processor : public Steinberg::Vst::AudioEffect
{
public:
	delay2Processor ();
	~delay2Processor () SMTG_OVERRIDE;

    static const int kNumTaps = 4;
    
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
    int m_circularBufferSampleRate;
    std::vector<CircularBuffer> m_dBuffer;
    
    Steinberg::Vst::ParamValue m_gainMaster = 0.f;
    
    Steinberg::Vst::ParamValue m_DryMix = 0.f;
    Steinberg::Vst::ParamValue m_WetMix = 0.f;
    
    Steinberg::Vst::ParamValue m_Delay1 = 0.f;
    Steinberg::Vst::ParamValue m_dGain1 = 0.f;
    Steinberg::Vst::ParamValue m_dFeedback1 = 0.f;
    
    Steinberg::Vst::ParamValue m_Delay2 = 0.f;
    Steinberg::Vst::ParamValue m_dGain2 = 0.f;
    Steinberg::Vst::ParamValue m_dFeedback2 = 0.f;
    
    Steinberg::Vst::ParamValue m_Delay3 = 0.f;
    Steinberg::Vst::ParamValue m_dGain3 = 0.f;
    Steinberg::Vst::ParamValue m_dFeedback3 = 0.f;
    
    Steinberg::Vst::ParamValue m_Delay4 = 0.f;
    Steinberg::Vst::ParamValue m_dGain4 = 0.f;
    Steinberg::Vst::ParamValue m_dFeedback4 = 0.f;
    
    
private:
    // Allpass filter coefficients
    double m_AllpassCoefficient;
    double m_PreviousInput;
    double m_PreviousOutput;

    // Function to calculate allpass filter coefficients
    void calculateAllpassCoefficient(double delayTimeSeconds);

    // Allpass filter processing function
    double processAllpass(double input);
};

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
