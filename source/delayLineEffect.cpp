//
//  delayLineEffect.cpp
//  delay_one
//
//  Created by Oberon Day-West on 27/07/2023.
//
#include <vector>
#include "delayLineEffect.h"

void DelayEffect::applyEffect()
{
    for (int i = 0; i < numSamples; ++i)
    {
        const float in = channelData[i];
        float out = 0.0;

        // The output is the input plus the contents of the
        // delay buffer (weighted by the mix levels).
        out = (dryMix * in + wetMix * delayData[dpr]);

        // Store the current information in the delay buffer.
        // delayData[dpr] is the delay sample we just read, i.e.
        // what came out of the buffer. delayData[dpw] is what
        // we write to the buffer, i.e. what goes in
        delayData[dpw] = in + (delayData[dpr] * feedback);

        if (++dpr >= delayBufLength)
            dpr = 0;

        if (++dpw >= delayBufLength)
            dpw = 0;

        // Store output sample in buffer, replacing the input
        channelData[i] = out;
    }
}

