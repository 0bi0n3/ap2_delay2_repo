//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

enum AudioParams : Steinberg::Vst::ParamID
{
    kParamGainId_Master = 102, // should be a unique id
    kParamDryMixId = 103,
    kParamWetMixId = 104,
    
    kParamDelayLengthId_Tap1 = 105,
    kParamDelayGainId_Tap1 = 106,
    kParamFeedbackId_Tap1 = 107,
    
    kParamDelayLengthId_Tap2 = 108,
    kParamDelayGainId_Tap2 = 109,
    kParamFeedbackId_Tap2 = 110,
    
    kParamDelayLengthId_Tap3 = 111,
    kParamDelayGainId_Tap3 = 112,
    kParamFeedbackId_Tap3 = 113,
    
    kParamDelayLengthId_Tap4 = 114,
    kParamDelayGainId_Tap4 = 115,
    kParamFeedbackId_Tap4 = 116,
    
};

namespace delayEffectProcessor {
//------------------------------------------------------------------------
static const Steinberg::FUID kdelay2ProcessorUID (0x13064EA4, 0x6325587E, 0xA4C5EA2C, 0xA0335013);
static const Steinberg::FUID kdelay2ControllerUID (0x161AA0FB, 0xAE725FD4, 0xBF4B4D10, 0x39753E74);

#define delay2VST3Category "Fx"

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
