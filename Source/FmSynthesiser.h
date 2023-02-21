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
public:
    
    FmSynthesiser(int numOperators);
    
    void prepareToPlay(double sampleRate);
    
    void processNextBlock(juce::dsp::AudioBlock<float> block);
    
    FmOperator* getOperator();
    
private:
    
    FmOperator m_operator;
};
