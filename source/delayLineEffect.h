//
//  delayLineEffect.hpp
//  delay_one
//
//  Created by Oberon Day-West on 27/07/2023.
//

#ifndef delayLineEffect_hpp
#define delayLineEffect_hpp

#include <stdio.h>
#include <vector>

class DelayEffect
{
public:
    DelayEffect(int numSamples,
                std::vector<float>& channelData,
                std::vector<float>& delayData,
                int delayBufLength,
                float dryMix,
                float wetMix,
                float feedback)
                :
                numSamples(numSamples),
                channelData(channelData),
                delayData(delayData),
                delayBufLength(delayBufLength),
                dryMix(dryMix),
                wetMix(wetMix),
                feedback(feedback)
    {
        dpr = 0;
        dpw = 0;
    }

    void applyEffect();
    
private:
    int numSamples;
    std::vector<float>& channelData;
    std::vector<float>& delayData;
    int delayBufLength;
    int dpr;
    int dpw;
    float dryMix;
    float wetMix;
    float feedback;
};


#endif /* delayLineEffect_hpp */
