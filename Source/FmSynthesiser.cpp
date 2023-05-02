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
//    This constructor allows the user to choose how many operators
//    they want this synthesiser to have
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators.push_back(440);
    }
}


void FmSynthesiser::prepareToPlay(double sampleRate)
{
//    This function sets the sample rate for the operators
//    It is important to call this function so that the operators work correctly
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].prepareToPlay(sampleRate);
    }
}

void FmSynthesiser::processNextBlock(juce::dsp::AudioBlock<float> block)
{
//    This function is the main function of this class
//    It takes in an AudioBlock as an argument and uses sample by sample
//    processing to generate the output
    auto* leftChannel = block.getChannelPointer(0);
    auto* rightChannel = block.getChannelPointer(1);
    
//    Pointers to each of the operators are declared
    auto* operator01 = getOperator(0);
    auto* operator02 = getOperator(1);
    auto* operator03 = getOperator(2);
    auto* operator04 = getOperator(3);
    
//    Here to sample by sample processing is changed depending on the
//    selected algorithm, refer to the documentation to see each of the
//    topologies
    
    switch (m_algorithm.get()) {
        
        //Topology 1
        case 1:
//            This for loop goes through all of the samples in the block
            for (int n = 0; n < block.getNumSamples(); ++n)
            {
                auto op04Sample = operator04->getNextSample();
//                As an example, here the sample from operator04 is
//                used to modulate operator03
                operator03->setModSample(op04Sample);
                auto op03Sample = operator03->getNextSample();
            
                operator02->setModSample(op03Sample);
                auto op02Sample = operator02->getNextSample();
            
                operator01->setModSample(op02Sample);
                auto op01Sample = operator01->getNextSample();
                
//                The carrier sample should not go above one
//                so this line serves as a limiter
                if (op01Sample > 1) op01Sample = 1;
                
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
                
                if (op01Sample > 1) op01Sample = 1;
                
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
                
                if (op01Sample > 1) op01Sample = 1;
                
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
                
                if (op01Sample > 1) op01Sample = 1;
                
                leftChannel[n] = op01Sample;
                rightChannel[n] = op01Sample;
                
            }
            
            break;
        default:
//            This is here as a failsafe just incase something goes wrong
//            but in an ideal scenario the case should never be default
            for (int n = 0; n < block.getNumSamples(); ++n)
            {
                leftChannel[n] = 0;
                rightChannel[n] = 0;
            }
            break;
    }
}

void FmSynthesiser::noteOn()
{
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].getAmpEnvelope()->noteOn();
        m_operators[i].getFreqEnvelope()->noteOn();
    }
}

void FmSynthesiser::noteOff()
{
    for (int i = 0; i < m_numOperators; ++i)
    {
        m_operators[i].getAmpEnvelope()->noteOff();
        m_operators[i].getFreqEnvelope()->noteOff();
    }
}

void FmSynthesiser::changeAlgorithm(int index)
{
    m_algorithm.set(index);
}

FmOperator* FmSynthesiser::getOperator(int index)
{
    return &m_operators[index];
}


