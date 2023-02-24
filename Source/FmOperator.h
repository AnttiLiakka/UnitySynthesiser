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
    
    void setFrequency(float& frequency);
    
    void setModSample(float& sample);
    
    void setModDepth(float& depth);
    
    void setAttack(float& attack);
    
    void setDecay(float& decay);
    
    void setSustain(float& sustain);
    
    void setRelease(float& release);
    
    float getAttack();
    
    float getDecay();
    
    float getSustain();
    
    float getRelease();
    
    juce::ADSR* getEnvelope();
    
private:
    
    juce::ADSR m_envelope;
    
    double m_currentAngle = 0;
    double m_angleDelta = 0;
    double m_sampleRate = 0;
    
    float m_frequency;
    float m_baseFrequency;
    float m_modulationSample = 0;
    float m_modulationDepth = 1;
    
    float m_attack = 1;
    float m_decay = 1;
    float m_sustain = 0.5f;
    float m_release = 1;
    
    juce::ADSR::Parameters m_envelopeParameters;
};
