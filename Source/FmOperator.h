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
    
private:
    
    juce::ADSR m_ampEnvelope;
    
    double m_currentAngle = 0;
    double m_angleDelta = 0;
    double m_sampleRate = 0;
    
    float m_frequency;
    float m_baseFrequency;
    float m_modulationSample = 0;
    float m_modulationDepth = 1;
    
    float m_ampAttack = 0;
    float m_ampDecay = 0;
    float m_ampSustain = 0;
    float m_ampRelease = 0;
    
    juce::Atomic<float> m_test = 0;
    
};
