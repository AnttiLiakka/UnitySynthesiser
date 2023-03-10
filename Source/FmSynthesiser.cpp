/*
  ==============================================================================

    FmSynthesiser.cpp
    Created: 21 Feb 2023 2:08:45pm
    Author:  Antti

  ==============================================================================
*/

#include "FmSynthesiser.h"

FmSynthesiser::FmSynthesiser(const int numOperators) : m_numOperators(numOperators)
{
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators.push_back(440);
    }
}


void FmSynthesiser::prepareToPlay(double sampleRate)
{    
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].prepareToPlay(sampleRate);
    }
}

void FmSynthesiser::processNextBlock(juce::dsp::AudioBlock<float> block)
{
    auto* leftChannel = block.getChannelPointer(0);
    auto* rightChannel = block.getChannelPointer(1);
    
    auto* operator01 = getOperator(0);
    auto* operator02 = getOperator(1);
    auto* operator03 = getOperator(2);
    auto* operator04 = getOperator(3);
    
    switch (m_algorithm) {
        
        //Topology 1
        case 1:
            
            for (int n = 0; n < block.getNumSamples(); ++n)
            {
                auto op04Sample = operator04->getNextSample();
            
                operator03->setModSample(op04Sample);
                auto op03Sample = operator03->getNextSample();
            
                operator02->setModSample(op03Sample);
                auto op02Sample = operator02->getNextSample();
            
                operator01->setModSample(op02Sample);
                auto op01Sample = operator01->getNextSample();
                
                if (op01Sample > 1) jassertfalse;
                
                leftChannel[n] = op01Sample;
                rightChannel[n] = op01Sample;
            
            }
            break;
        
        //Topology 2
        case 2:
            
            for (int n = 0; n < block.getNumSamples(); ++n)
            {
                auto op03Sample = operator03->getNextSample();
                
                auto op04Sample = operator03->getNextSample();
                
                float modSample = op03Sample + op04Sample;
                operator02->setModSample(modSample);
                auto op02Sample = operator02->getNextSample();
                
                operator01->setModSample(op02Sample);
                auto op01Sample = operator01->getNextSample();
                
                if (op01Sample > 1) jassertfalse;
                
                leftChannel[n] = op01Sample;
                rightChannel[n] = op01Sample;
                
            }
            break;
        //Topology 3
        case 3:
            
            for (int n = 0; n < block.getNumSamples(); ++n)
            {
                auto op03Sample = operator03->getNextSample();
                
                auto op04Sample = operator04->getNextSample();
                
                operator02->setModSample(op03Sample);
                auto op02Sample = operator02->getNextSample();
                
                float modSample = op02Sample + op04Sample;
                operator01->setModSample(modSample);
                auto op01Sample = operator01->getNextSample();
                
                if (op01Sample > 1) jassertfalse;
                
                leftChannel[n] = op01Sample;
                rightChannel[n] = op01Sample;
                
            }
            break;
        //Topology 4
        case 4:
            
            for (int n = 0; n < block.getNumSamples(); ++n)
            {
                auto op04Sample = operator04->getNextSample();
                
                operator02->setModSample(op04Sample);
                operator03->setModSample(op04Sample);
                
                auto op03Sample = operator03->getNextSample();
                auto op02Sample = operator02->getNextSample();
                
                float modSample = op02Sample + op03Sample;
                
                operator01->setModSample(modSample);
                
                auto op01Sample = operator01->getNextSample();
                
                if (op01Sample > 1) jassertfalse;
                
                leftChannel[n] = op01Sample;
                rightChannel[n] = op01Sample;
                
            }
            
            break;
        //Epic fail
        default:
            jassertfalse; //Change to return on release
            break;
    }
}

void FmSynthesiser::noteOn()
{
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].getAmpEnvelope()->noteOn();
    }
}

void FmSynthesiser::noteOff()
{
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].getAmpEnvelope()->noteOff();
    }
}

void FmSynthesiser::changeAlgorithm(int index)
{
    m_algorithm = index;
}

FmOperator* FmSynthesiser::getOperator(int index)
{
    return &m_operators[index];
}


