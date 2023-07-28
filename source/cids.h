//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

enum AudioParams : Steinberg::Vst::ParamID
{
    kParamGainId = 102, // should be a unique id
    kParamDelayLengthId = 103,
    kParamDryMixId = 104,
    kParamWetMixId = 105,
    kParamFeedbackId = 106,
    kParamNumParametersId = 107,
};

namespace delayEffectProcessor {
//------------------------------------------------------------------------
static const Steinberg::FUID kdelay2ProcessorUID (0x13064EA4, 0x6325587E, 0xA4C5EA2C, 0xA0335013);
static const Steinberg::FUID kdelay2ControllerUID (0x161AA0FB, 0xAE725FD4, 0xBF4B4D10, 0x39753E74);

#define delay2VST3Category "Fx"

//------------------------------------------------------------------------
} // namespace delayEffectProcessor
