/*
  ==============================================================================

    FmOperator.cpp
    Created: 21 Feb 2023 1:47:15pm
    Author:  Antti

  ==============================================================================
*/

#include "FmOperator.h"

FmOperator::FmOperator(float baseFrequency) :
                                            m_frequency(baseFrequency),
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
    
    m_ampEnvelope.setSampleRate(sampleRate);
}

float FmOperator::getNextSample()
{
    auto currentSample = (float) std::sin(m_currentAngle) * (m_modulationDepth * m_ampEnvelope.getNextSample());
    m_currentAngle += m_angleDelta;
    
    m_frequency = m_baseFrequency + (m_modulationSample);
    updateAngleDelta();
    
    return currentSample;
}

void FmOperator::setFrequency(float frequency)
{
    m_baseFrequency = frequency;
}

void FmOperator::setModSample(float sample)
{
    m_modulationSample = sample;
}

void FmOperator::setModDepth(float depth)
{
    m_modulationDepth = depth;
}

void FmOperator::updateAmpEnvParams(float attack, float decay, float sustain, float release)
{
    m_ampEnvelope.setParameters(juce::ADSR::Parameters(attack, decay, sustain, release));
}

juce::ADSR* FmOperator::getAmpEnvelope()
{
    return &m_ampEnvelope;
}
