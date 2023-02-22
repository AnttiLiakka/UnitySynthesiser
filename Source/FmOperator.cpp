/*
  ==============================================================================

    FmOperator.cpp
    Created: 21 Feb 2023 1:47:15pm
    Author:  Antti

  ==============================================================================
*/

#include "FmOperator.h"

FmOperator::FmOperator(float baseFrequency) : m_frequency(baseFrequency),
                                              m_baseFrequency(baseFrequency)
{
    
}

void FmOperator::updateAngleDelta()
{
    auto cyclesPerSample = m_frequency / m_sampleRate;
    m_angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}

void FmOperator::prepareToPlay(double sampleRate)
{
    m_sampleRate = sampleRate;
    updateAngleDelta();
}

float FmOperator::getNextSample(float modulationSample, double modulationDepth)
{
    auto currentSample = (float) std::sin(m_currentAngle) * modulationDepth;
    m_currentAngle += m_angleDelta;
    
    m_frequency = m_baseFrequency + modulationSample;
    updateAngleDelta();
    
    return currentSample;
}

void FmOperator::setFrequency(float& frequency)
{
    m_baseFrequency = frequency;
}
