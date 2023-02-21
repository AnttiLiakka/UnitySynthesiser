/*
  ==============================================================================

    FmSynthesiser.cpp
    Created: 21 Feb 2023 2:08:45pm
    Author:  Antti

  ==============================================================================
*/

#include "FmSynthesiser.h"

FmSynthesiser::FmSynthesiser(int numOperators) : m_operator(440.0f)
{

}


void FmSynthesiser::prepareToPlay(double sampleRate)
{
    m_operator.prepareToPlay(sampleRate);
}

void FmSynthesiser::processNextBlock(juce::dsp::AudioBlock<float> block)
{
    auto* leftChannel = block.getChannelPointer(0);
    auto* rightChannel = block.getChannelPointer(1);
    
    for (int n = 0; n < block.getNumSamples(); ++n)
    {
        auto currentSample = m_operator.getNextSample(0);
        
        leftChannel[n] = currentSample;
        rightChannel[n] = currentSample;
    }

}

FmOperator* FmSynthesiser::getOperator()
{
    return &m_operator;
}


