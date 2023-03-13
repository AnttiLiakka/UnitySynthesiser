/*
  ==============================================================================

    FmOperator.h
    Created: 21 Feb 2023 1:47:15pm
    Author:  Antti

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FmOperator
{
  
public:
    
    FmOperator(float baseFrequency);
    
    void updateAngleDelta();
    
    void prepareToPlay(double sampleRate);
    
    float getNextSample();
    
    void setFrequency(float frequency);
    
    void setModSample(float sample);
    
    void setModDepth(float depth);
    
    void setAmpAttack(float attack);
    
    void setAmpDecay(float decay);
    
    void setAmpSustain(float sustain);
    
    void setAmpRelease(float release);
    
    float getAmpAttack();
    
    float getAmpDecay();
    
    float getAmpSustain();
    
    float getAmpRelease();
    
    void updateAmpEnvelopeParameters();
    
    juce::ADSR* getAmpEnvelope();
    
    void setFreqAttack(float attack);
    
    void setFreqDecay(float decay);
    
    void setFreqSustain(float sustain);
    
    void setFreqRelease(float release);
    
    float getFreqAttack();
    
    float getFreqDecay();
    
    float getFreqSustain();
    
    float getFreqRelease();
    
    void updateFreqEnvelopeParameters();
    
    juce::ADSR* getFreqEnvelope();
    
private:
    
    double m_currentAngle = 0;
    double m_angleDelta = 0;
    double m_sampleRate = 0;
    double m_frequency;
    juce::Atomic<double> m_baseFrequency;
    float m_modulationSample = 0;
    float m_modulationDepth = 1;
    
    float m_ampAttack = 0;
    float m_ampDecay = 0;
    float m_ampSustain = 0;
    float m_ampRelease = 0;
    
    float m_freqAttack = 0;
    float m_freqDecay = 0;
    float m_freqSustain = 0;
    float m_freqRelease = 0;
    
    juce::ADSR m_ampEnvelope;
    juce::ADSR m_freqEnvelope;
    
};
