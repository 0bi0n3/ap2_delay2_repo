//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
// This script was adapted and referenced from Reiss and McPherson (2015) Tarr (2019) and Roma (2023).
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
    
    // Set an initial value for the allpass filter coefficient
    m_AllpassCoefficient = 0.5;
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
    Steinberg::Vst::SpeakerArrangement arr;
    if (getBusArrangement(Steinberg::Vst::kOutput, 0, arr) != kResultTrue)
        return kResultFalse;
    
    int numChannels = Steinberg::Vst::SpeakerArr::getChannelCount(arr);
    double sampleRate = processSetup.sampleRate;
    m_circularBufferSampleRate = sampleRate;
    
    // Function to initialize the CircularBuffer for each channel
    auto initializeCircularBuffer = [sampleRate]()
    {
        return CircularBuffer(sampleRate * 2); // maximum delay time for each tap
    };
    
    if (state)
    {
        // Create a new CircularBuffer for each channel and store it in m_dBuffer
        m_dBuffer.clear();
        for (int i = 0; i < numChannels; i++)
        {
            m_dBuffer.push_back(initializeCircularBuffer());
        }
    }
    else
    {
        // Clear m_dBuffer when the plugin is disabled (Off)
        m_dBuffer.clear();
    }
    
    return AudioEffect::setActive(state);
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
                switch (paramQueue->getParameterId())
                {
                    // Master Gain parameter
                    case AudioParams::kParamGainId_Master:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update our internal gain value with the new value
                            m_gainMaster = value;
                        }
                        break;

                    // Delay Time for Tap 1
                    case AudioParams::kParamDelayLengthId_Tap1:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update internal delay length value for Tap 1 with the new value
                            m_Delay1 = value;
                        }
                        break;

                    // Delay Gain for Tap 1
                    case kParamDelayGainId_Tap1:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the delay gain value for Tap 1 with the new value
                            m_dGain1 = value;
                        }
                        break;

                    // Feedback Gain for Tap 1
                    case kParamFeedbackId_Tap1:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the feedback gain value for Tap 1 with the new value
                            m_dFeedback1 = value;
                        }
                        break;

                    // Delay Time for Tap 2
                    case kParamDelayLengthId_Tap2:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update internal delay length value for Tap 2 with the new value
                            m_Delay2 = value;
                        }
                        break;

                    // Delay Gain for Tap 2
                    case kParamDelayGainId_Tap2:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the delay gain value for Tap 2 with the new value
                            m_dGain2 = value;
                        }
                        break;

                    // Feedback Gain for Tap 2
                    case kParamFeedbackId_Tap2:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the feedback gain value for Tap 2 with the new value
                            m_dFeedback2 = value;
                        }
                        break;

                    // Delay Time for Tap 3
                    case kParamDelayLengthId_Tap3:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update internal delay length value for Tap 3 with the new value
                            m_Delay3 = value;
                        }
                        break;

                    // Delay Gain for Tap 3
                    case kParamDelayGainId_Tap3:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the delay gain value for Tap 3 with the new value
                            m_dGain3 = value;
                        }
                        break;

                    // Feedback Gain for Tap 3
                    case kParamFeedbackId_Tap3:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the feedback gain value for Tap 3 with the new value
                            m_dFeedback3 = value;
                        }
                        break;

                    // Delay Time for Tap 4
                    case kParamDelayLengthId_Tap4:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update internal delay length value for Tap 4 with the new value
                            m_Delay4 = value;
                        }
                        break;

                    // Delay Gain for Tap 4
                    case kParamDelayGainId_Tap4:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the delay gain value for Tap 4 with the new value
                            m_dGain4 = value;
                        }
                        break;

                    // Feedback Gain for Tap 4
                    case kParamFeedbackId_Tap4:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update the feedback gain value for Tap 4 with the new value
                            m_dFeedback4 = value;
                        }
                        break;

                    // Dry Mix parameter
                    case AudioParams::kParamDryMixId:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update our internal dry mix value with the new value
                            m_DryMix = value;
                        }
                        break;

                    // Wet Mix parameter
                    case AudioParams::kParamWetMixId:
                        // Get the most recent value of the parameter
                        if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
                        {
                            // Update our internal wet mix value with the new value
                            m_WetMix = value;
                        }
                        break;
                }

            }
        }
    }
    
    // If there's no input or samples, there's nothing to process
    if (data.numInputs == 0 || data.numSamples == 0)
        return kResultOk;

    // Get basic information about the sound data
    int32 numChannels = data.inputs[0].numChannels;
    uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
    
    // These are pointers to the actual audio data for processing
    void** in = getChannelBuffersPointer(processSetup, data.inputs[0]);
    void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);

    // Make sure output isn't marked as silent
    data.outputs[0].silenceFlags = 0;

    // Determine the mix of original (dry) and effect (wet) sounds
    double dryMix = (1.0 - m_WetMix);
    double wetMix = m_WetMix;
    double gainLimitter = 1.0 - wetMix * 0.5;

    // Process each channel of audio separately
    for (int32 i = 0; i < numChannels; i++)
    {

        // Take samples from input audio
        int32 samples = data.numSamples;
        Vst::Sample32* ptrIn = (Vst::Sample32*)in[i];
        Vst::Sample32* ptrOut = (Vst::Sample32*)out[i];
        Vst::Sample32 outputAudio;

        while (--samples >= 0) {

            // Read from the input and write to the output
            double inputAudio = (*ptrIn++);

            // Determine the minimum delay based on the buffer sample rate
            const double bufferDelay = 1.0 / m_circularBufferSampleRate;

            // Calculate delay times for each of the taps
            double delayedTime1 = std::max(static_cast<double>(m_Delay1), bufferDelay);
            double delayedTime2 = std::max(static_cast<double>(m_Delay2 * m_Delay1), bufferDelay);
            double delayedTime3 = std::max(static_cast<double>(m_Delay3 * m_Delay1), bufferDelay);
            double delayedTime4 = std::max(static_cast<double>(m_Delay4 * m_Delay1), bufferDelay);

            // Get a delayed signal for each tap
            double delayedSig1 = m_dBuffer[i].performInterpolation(m_circularBufferSampleRate * delayedTime1);
            double delayedSig2 = m_dBuffer[i].performInterpolation(m_circularBufferSampleRate * delayedTime2);
            double delayedSig3 = m_dBuffer[i].performInterpolation(m_circularBufferSampleRate * delayedTime3);
            double delayedSig4 = m_dBuffer[i].performInterpolation(m_circularBufferSampleRate * delayedTime4);

            // Each delayed signal is fed back into itself to create an echo
            double feedbackSig1 = m_dFeedback1 * delayedSig1;
            double feedbackSig2 = m_dFeedback2 * delayedSig2;
            double feedbackSig3 = m_dFeedback3 * delayedSig3;
            double feedbackSig4 = m_dFeedback4 * delayedSig4;

            // We limit the total feedback gain to avoid overflows
            const double maxFeedbackGain = 0.8;
            double feedbackGain1 = std::min(m_dFeedback1, maxFeedbackGain);
            double feedbackGain2 = std::min(m_dFeedback2, maxFeedbackGain);
            double feedbackGain3 = std::min(m_dFeedback3, maxFeedbackGain);
            double feedbackGain4 = std::min(m_dFeedback4, maxFeedbackGain);

            // Total feedback is the sum of each feedback gain times its delayed signal
            double mixedFeedbackSignal = feedbackGain1 * delayedSig1 + feedbackGain2 * delayedSig2
                               + feedbackGain3 * delayedSig3 + feedbackGain4 * delayedSig4;

            // Gain limiter applied to the mixed feedback signal
            double mixedFeedbackLimited = gainLimitter * mixedFeedbackSignal;
            
            // Mix the input audio with the feedback and write it back into the buffer
            m_dBuffer[i].performWrite(inputAudio + mixedFeedbackLimited);

            // Calculate the feedback gain for each tap
            double fbGain1 = m_dGain1 * delayedSig1;
            double fbGain2 = m_dGain2 * delayedSig2;
            double fbGain3 = m_dGain3 * delayedSig3;
            double fbGain4 = m_dGain4 * delayedSig4;

            // Sum of all feedback gains is the total signal
            double totalSignal = fbGain1 + fbGain2 + fbGain3 + fbGain4;

            // Mix the dry (original) audio with the wet (effected) audio
            double mixedAudio = (dryMix * inputAudio) + (wetMix * totalSignal);

            // Gain limiter applied to the mixed audio
            outputAudio = gainLimitter * mixedAudio;

            // Allpass filter applied to the output
            double allpassOutput = processAllpass(outputAudio);

            // The result is written to the output
            (*ptrOut++) = allpassOutput * m_gainMaster;
        }
    }

    return kResultOk;
}


//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
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

    // called when we load a preset or project, the model has to be reloaded
    IBStreamer streamer (state, kLittleEndian);

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Processor::getState (IBStream* state)
{
    // here we need to save the model (preset or project)
    IBStreamer streamer (state, kLittleEndian);
    return kResultOk;
}

// Function to calculate the coefficient for the allpass filter
void delay2Processor::calculateAllpassCoefficient(double delayTimeSeconds)
{
    // Calculate the coefficient 'g' for the allpass filter using the desired delay time in seconds.
    // The formula is: g = (1 - delayTimeSeconds) / (1 + delayTimeSeconds)
    double g = (1.0 - delayTimeSeconds) / (1.0 + delayTimeSeconds);

    // Store the calculated coefficient in the member variable 'm_AllpassCoefficient' for later use.
    m_AllpassCoefficient = g;
}

// Function to process the input signal through the allpass filter and return the filtered output
double delay2Processor::processAllpass(double input)
{
    // Multiply the input by the negative of 'm_AllpassCoefficient' and add the previous input value.
    double output = input * (-m_AllpassCoefficient) + m_PreviousInput;

    // Add the previous output value multiplied by 'm_AllpassCoefficient'.
    output += m_AllpassCoefficient * m_PreviousOutput;

    // Update the previous input and output values for the next sample processing.
    m_PreviousInput = input;
    m_PreviousOutput = output;

    // Return the filtered output from the allpass filter.
    return output;
}


//------------------------------------------------------------------------
} // namespace delayEffectProcessor
