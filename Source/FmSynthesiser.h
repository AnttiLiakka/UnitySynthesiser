/*
  ==============================================================================

    FmSynthesiser.h
    Created: 21 Feb 2023 2:08:45pm
    Author:  Antti

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FmOperator.h"

class FmSynthesiser
{
    friend class UnitySynthesiserAudioProcessor;
public:
    
    FmSynthesiser(int numOperators);
    
    void prepareToPlay(double sampleRate);
    
    void processNextBlock(juce::dsp::AudioBlock<float> block);
    
    FmOperator* getOperator(int index);
    
private:
    
    std::vector<FmOperator> m_operators;
    int m_numOperators;
    
    float m_operator01Freq = 440;
    float m_operator01Depth = 1;
    
    float m_operator02Freq = 0;
    float m_operator02Depth = 1;
};
