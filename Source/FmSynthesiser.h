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

///This class is a FM synthesiser containing a vector of FmOperators
class FmSynthesiser
{
public:
    ///The constructor, it takes in an interger argument to set the m_numOperators member
    FmSynthesiser(const int numOperators);
    ///This function sets the sample rate for the operators
    void prepareToPlay(double sampleRate);
    ///This function processes an AudioBlock by calculating the output sample for each channel based on the algorithm selected
    void processNextBlock(juce::dsp::AudioBlock<float> block);
    ///This function starts the attack phase of its operators amplitude and frequency envelopes
    void noteOn();
    ///This function starts the release phase of its operators amplitude and frequency envelopes
    void noteOff();
    ///This function changes the topology algorithm
    void changeAlgorithm(int index);
    ///This function returns a pointer to an operator from the m_operators vector
    FmOperator* getOperator(int index);
    
private:
    ///This member is the vector containing the operators
    std::vector<FmOperator> m_operators;
    ///This member is the number of operators this class has
    const int m_numOperators;
    ///This member is the algorithm this class is using. Needs to be threadsafe
    juce::Atomic<int> m_algorithm { 1 };
    
};
