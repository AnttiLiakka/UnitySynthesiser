/*
  ==============================================================================

    FmSynthesiser.cpp
    Created: 21 Feb 2023 2:08:45pm
    Author:  Antti

  ==============================================================================
*/

#include "FmSynthesiser.h"

FmSynthesiser::FmSynthesiser(int numOperators) : m_numOperators(numOperators)
{
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators.push_back(440);
    }
}


void FmSynthesiser::prepareToPlay(double sampleRate)
{
    getOperator(0)->setFrequency(m_operator01Freq);
    getOperator(1)->setFrequency(m_operator02Freq);
    
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].prepareToPlay(sampleRate);
    }
}

void FmSynthesiser::processNextBlock(juce::dsp::AudioBlock<float> block)
{
    auto* leftChannel = block.getChannelPointer(0);
    auto* rightChannel = block.getChannelPointer(1);
    
    for (int n = 0; n < block.getNumSamples(); ++n)
    {
        auto* operator01 = getOperator(0);
        auto* operator02 = getOperator(1);
        
        auto op02Sample = operator02->getNextSample(0, m_operator02Depth);
        
        auto currentSample = operator01->getNextSample(op02Sample);
        
        if(currentSample > 1) jassertfalse;
        
        leftChannel[n] = currentSample;
        rightChannel[n] = currentSample;
    }

}

FmOperator* FmSynthesiser::getOperator(int index)
{
    return &m_operators[index];
}


