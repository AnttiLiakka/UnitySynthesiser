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
    
    float getNextSample(float modulationSample, double modulationDepth = 1);
    
    void setFrequency(float& frequency);
    
private:
    
    double m_currentAngle = 0;
    double m_angleDelta = 0;
    double m_sampleRate = 0;
    
    float m_frequency;
    float m_baseFrequency;
    
};
