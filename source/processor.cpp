//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
// This script was adapted and referenced from Reiss and McPherson (2015) and Tarr (2019).
// Please refer to accompanying report reference list for full reference details.
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace delayEffectProcessor {
//------------------------------------------------------------------------
// delay2Processor
//------------------------------------------------------------------------
delay2Processor::delay2Processor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kdelay2ControllerUID);
    
    // set initial delay line values
    m_delayBufferLength = 44100; // delay length/time
    m_delayLength = 0.5f;
    m_dryMix = 1.0f;
    m_wetMix = 0.5f;
    m_feedback = 0.75f;
    
    m_delayReadPosition = 0;
    m_delayWritePosition = 0;
    
    // initialize delay buffer
    m_delayBuffer.resize(2, std::vector<float>(m_delayBufferLength, 0.0f));
}

//------------------------------------------------------------------------
delay2Processor::~delay2Processor ()
{
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::process (Vst::ProcessData& data)
{
    // Check if there are any changes in the input parameters
    if (data.inputParameterChanges)
    {
        // Get the number of parameters that have changed
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();

        // Iterate through each changed parameter
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            // Get the queue of parameter values for this parameter
            Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData (index);

            // If the queue is valid, we process the changes
            if (paramQueue)
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();

                // Check which parameter has changed
                switch (paramQueue->getParameterId ())
                {
                    // In this case, we're only handling changes to gain
                    case AudioParams::kParamGainId:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
                            // Update our internal gain value with the new value
                            mGain = value;
                        break;
                }
            }
        }
    }
    
    // If there's no data to process, return
    if (data.numInputs == 0 || data.numSamples == 0)
        return kResultOk;

    // Get the number of channels and samples
    int32 numChannels = data.inputs[0].numChannels;
    int32 numSamples = data.numSamples;
    
    // Initialize delay buffer pointers
    int32 dpr = m_delayReadPosition;
    int32 dpw = m_delayWritePosition;
    
    // Get input and output buffer pointers
    void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
    void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

    // Initially set the output silence flag to 0, assuming there will be sound
    data.outputs[0].silenceFlags = 0;

    // Iterate over each channel
    for (int32 channel = 0; channel < numChannels; ++channel)
    {
        // Get input data for this channel
        Vst::Sample32* channelData = static_cast<Vst::Sample32*>(in[channel]);
        
        // Get output data for this channel
        Vst::Sample32* outputData = static_cast<Vst::Sample32*>(out[channel]);

        // Get delay buffer data for this channel
        float* delayData = m_delayBuffer[std::min(channel, (int)m_delayBuffer.size() - 1)].data();
        
        // Process each sample
        for (int32 i = 0; i < numSamples; ++i)
        {
            // Get the current input sample
            const float in = channelData[i];
            float out = 0.0;

            // Create an output sample based on input, delay buffer content, and dry/wet mix
            out = (m_dryMix * in + m_wetMix * delayData[dpr]);

            // Write the current input and feedback-scaled delay buffer content to the delay buffer
            delayData[dpw] = in + (delayData[dpr] * m_feedback);
            
            // Increment and wrap the delay buffer pointers
            if (++dpr >= m_delayBufferLength)
                dpr = 0;
            if (++dpw >= m_delayBufferLength)
                dpw = 0;
            
            // Write the output sample to the output data
            outputData[i] = out * mGain;
        }
    }
        
    // Update delay buffer pointers for the next block
    m_delayReadPosition = dpr;
    m_delayWritePosition = dpw;

    // If there are more output channels than input channels, clear the remaining channels
    for (int32 i = numChannels; i < data.numOutputs; ++i)
    {
        // Clear channel data by setting it to zero
        memset (out[i], 0, numSamples * sizeof(Vst::Sample32));
    }
    
    // Everything processed successfully
    return kResultOk;
}


//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
    // Check for sample rate changes
    if (m_sampleRate != newSetup.sampleRate)
    {
        m_sampleRate = newSetup.sampleRate;
        resizeDelayBuffer();
    }

    // Let the parent class also handle this call
    return AudioEffect::setupProcessing(newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::setState (IBStream* state)
{
    if (!state)
        return kResultFalse;

    // called when we load a preset or project, the model has to be reloaded
    IBStreamer streamer (state, kLittleEndian);
    float savedParam1 = 0.f;
    if (streamer.readFloat (savedParam1) == false)
        return kResultFalse;
    mGain = savedParam1;

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::getState (IBStream* state)
{
    // here we need to save the model (preset or project)
    float toSaveParam1 = mGain;
    IBStreamer streamer (state, kLittleEndian);
    streamer.writeFloat (toSaveParam1);
    return kResultOk;
}

void delay2Processor::resizeDelayBuffer()
{
    // Assuming you want a maximum delay of 1 second
    m_delayBufferLength = static_cast<int>(m_sampleRate * 2.0);
    
    // Calculate the position to read from the delay buffer based on the current settings
    // and make sure the read position wraps around in a circular manner using the modulo operator.
    m_delayReadPosition = static_cast<int>(m_delayWritePosition - (m_delayLength * m_sampleRate) + m_delayBufferLength) % m_delayBufferLength;

    // Initialize for two channels (stereo). Increase this if you want more channels.
    m_delayBuffer.resize(2, std::vector<float>(m_delayBufferLength, 0.0f));
}

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
