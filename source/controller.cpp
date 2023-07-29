//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#include "controller.h"
#include "cids.h"
#include "base/source/fstreamer.h"


using namespace Steinberg;

namespace delayEffectProcessor {

//------------------------------------------------------------------------
// delay2Controller Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

    //---Create Parameters------------
    parameters.addParameter(STR16("Master Gain"),
                            STR16("dB"),
                            0,
                            0.5,
                            Vst::ParameterInfo::kCanAutomate,
                            AudioParams::kParamGainId_Master,
                            0);

    parameters.addParameter(STR16("Delay Time Tap 1"),
                            STR16("sec"),
                            0,
                            0.5,
                            Vst::ParameterInfo::kCanAutomate,
                            AudioParams::kParamDelayLengthId_Tap1,
                            0);

    parameters.addParameter(STR16("Delay Gain Tap 1"),
                            STR16("dB"),
                            0,
                            0.5,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayGainId_Tap1,
                            0);

    parameters.addParameter(STR16("Feedback Gain Tap 1"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamFeedbackId_Tap1,
                            0);

    parameters.addParameter(STR16("Delay Time Tap 2"),
                            STR16("sec"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayLengthId_Tap2,
                            0);

    parameters.addParameter(STR16("Delay Gain Tap 2"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayGainId_Tap2,
                            0);

    parameters.addParameter(STR16("Feedback Gain Tap 2"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamFeedbackId_Tap2,
                            0);

    parameters.addParameter(STR16("Delay Time Tap 3"),
                            STR16("sec"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayLengthId_Tap3,
                            0);

    parameters.addParameter(STR16("Delay Gain Tap 3"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayGainId_Tap3,
                            0);

    parameters.addParameter(STR16("Feedback Gain Tap 3"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamFeedbackId_Tap3,
                            0);

    parameters.addParameter(STR16("Delay Time Tap 4"),
                            STR16("sec"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayLengthId_Tap4,
                            0);

    parameters.addParameter(STR16("Delay Gain Tap 4"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamDelayGainId_Tap4,
                            0);

    parameters.addParameter(STR16("Feedback Gain Tap 4"),
                            STR16("dB"),
                            0,
                            0.0,
                            Vst::ParameterInfo::kCanAutomate,
                            kParamFeedbackId_Tap4,
                            0);

    parameters.addParameter(STR16("Dry Mix"),
                            STR16("%"),
                            0,
                            0.5,
                            Vst::ParameterInfo::kCanAutomate,
                            AudioParams::kParamDryMixId,
                            0);

    parameters.addParameter(STR16("Wet Mix"),
                            STR16("%"),
                            0,
                            0.5,
                            Vst::ParameterInfo::kCanAutomate,
                            AudioParams::kParamWetMixId,
                            0);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::setComponentState (IBStream* state)
{
    // Here you get the state of the component (Processor part)
    if (!state)
        return kResultFalse;

//    IBStreamer streamer (state, kLittleEndian);
//    float   savedParam1 = 0.0f,
//            savedParam2 = 0.0f,
//            savedParam3 = 0.0f,
//            savedParam4 = 0.0f,
//            savedParam5 = 0.0f;
//
//    if (   streamer.readFloat (savedParam1) == false
//        || streamer.readFloat (savedParam2) == false
//        || streamer.readFloat (savedParam3) == false
//        || streamer.readFloat (savedParam4) == false
//        || streamer.readFloat (savedParam5) == false
//        )
//        return kResultFalse;
//
//    // sync with our parameter
//    if (auto param = parameters.getParameter (AudioParams::kParamGainId))
//        param->setNormalized (savedParam1);
//    if (auto param = parameters.getParameter (AudioParams::kParamDelayLengthId))
//        param->setNormalized (savedParam2);
//    if (auto param = parameters.getParameter (AudioParams::kParamDryMixId))
//        param->setNormalized (savedParam3);
//    if (auto param = parameters.getParameter (AudioParams::kParamWetMixId))
//        param->setNormalized (savedParam4);
//    if (auto param = parameters.getParameter (AudioParams::kParamFeedbackId))
//        param->setNormalized (savedParam5);

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API delay2Controller::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
        return nullptr;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API delay2Controller::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
