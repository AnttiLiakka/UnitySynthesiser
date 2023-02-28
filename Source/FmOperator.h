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
    
    void updateAmpEnvParams(float attack, float decay, float sustain, float release);
    
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
    
};
