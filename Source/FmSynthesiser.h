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
    
    FmSynthesiser(juce::AudioProcessorValueTreeState& parameters, const int numOperators);
    
    void prepareToPlay(double sampleRate);
    
    void processNextBlock(juce::dsp::AudioBlock<float> block);
    
    void noteOn();
    
    void noteOff();
    
    void changeAlgorithm(int index);
    
    FmOperator* getOperator(int index);
    
private:
    
    std::vector<FmOperator> m_operators;
    juce::AudioProcessorValueTreeState& m_parameters;
    
    const int m_numOperators;
    int m_algorithm = 1;
    
};
