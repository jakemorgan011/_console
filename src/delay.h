/*
  ==============================================================================

    delay.h
    Created: 11 Feb 2025 4:42:09pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "lfo.h"
#include "JuceHeader.h"
namespace termite {
class delay {
public:
    void prepareToPlay(){
        
    }
    void processBlock(){
        
    }
    void setParameters(){
        
    }
    
private:
    //juce::LinearSmoothedValue<float> smoothedTime;
    juce::linearSmoothedValue<float> smoothedFeedback;
    juce::linearSmoothedValue<float> smoothedDryWet;
    termite::sinlfo
    
};

}
