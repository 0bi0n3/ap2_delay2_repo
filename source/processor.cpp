//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
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
    delayBufferLength_ = 44100; // for example, a delay of 1 second at 44100Hz sample rate
    delayReadPosition_ = 0;
    delayWritePosition_ = 0;
    
    // initialize delay buffer
    delayBuffer.resize(2, std::vector<float>(delayBufferLength_, 0.0f));
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
    //--- First: Read inputs parameter changes-----------
        if (data.inputParameterChanges)
        {
            // for each parameter defined by its ID
            int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
            for (int32 index = 0; index < numParamsChanged; index++)
            {
                // for this parameter we could iterate the list of value changes (could 1 per audio block or more!)
                // in this example we get only the last value (getPointCount - 1)
                Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData (index);
                if (paramQueue)
                {
                    Vst::ParamValue value;
                    int32 sampleOffset;
                    int32 numPoints = paramQueue->getPointCount ();
                    switch (paramQueue->getParameterId ())
                    {
                        case AudioParams::kParamGainId:
                            if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
                                mGain = value;
                            break;
                    }
                }
            }
        }
	
	//--- Here you have to implement your processing
    //-- Flush case: we only need to update parameter, no processing possible
    if (data.numInputs == 0 || data.numSamples == 0)
        return kResultOk;

    //--- Here you have to implement your processing
    int32 numChannels = data.inputs[0].numChannels;
    
    int delayReadPtr;
    int delayWritePtr;

    //---get audio buffers using helper-functions(vstaudioprocessoralgo.h)-------------
    uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
    void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
    void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

    // Here could check the silent flags
    // now we will produce the output
    // mark our outputs has not silent
    data.outputs[0].silenceFlags = 0;

    float gain = mGain;
    // for each channel (left and right)
    for (int32 i = 0; i < numChannels; i++)
    {
        int32 samples = data.numSamples;
        Vst::Sample32* ptrIn = (Vst::Sample32*)in[i];
        Vst::Sample32* ptrOut = (Vst::Sample32*)out[i];
        Vst::Sample32 tmp;
        // for each sample in this channel
        while (--samples >= 0)
        {
            // apply gain
            tmp = (*ptrIn++) * gain;
            (*ptrOut++) = tmp;
        }
    }
    
    // Here could check the silent flags
    //---check if silence---------------
    // normally we have to check each channel (simplification)
    if (data.inputs[0].silenceFlags != 0)
    {
        // mark output silence too
        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
        
        // the plug-in has to be sure that if it sets the flags silence that the output buffer are clear
        for (int32 i = 0; i < numChannels; i++)
        {
            // do not need to be cleared if the buffers are the same (in this case input buffer are
            // already cleared by the host)
            if (in[i] != out[i])
            {
                memset (out[i], 0, sampleFramesSize);
            }
        }
        // nothing to do at this point
    }
    
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
    // Check for sample rate changes
    if (sampleRate_ != newSetup.sampleRate)
    {
        sampleRate_ = newSetup.sampleRate;
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
    delayBufferLength_ = static_cast<int>(sampleRate_ * 1.0);

    // Initialize for two channels (stereo). Increase this if you want more channels.
    delayBuffer.resize(2, std::vector<float>(delayBufferLength_, 0.0f));
}

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
