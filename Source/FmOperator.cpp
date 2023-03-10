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
    
    m_envelope.setSampleRate(sampleRate);
    updateEnvelopeParameters();
}

float FmOperator::getNextSample()
{
    auto currentSample = (float) std::sin(m_currentAngle) * (m_modulationDepth * m_envelope.getNextSample());
    m_currentAngle += m_angleDelta;
    
    m_frequency = m_baseFrequency + (m_modulationSample);
    updateAngleDelta();
    
    return currentSample;
}

void FmOperator::setFrequency(float& frequency)
{
    m_baseFrequency = frequency;
    updateEnvelopeParameters();
}

void FmOperator::setModSample(float& sample)
{
    m_modulationSample = sample;
    updateEnvelopeParameters();
}

void FmOperator::setModDepth(float& depth)
{
    m_modulationDepth = depth;
    updateEnvelopeParameters();
}

void FmOperator::setAttack(float& attack)
{
    m_attack = attack;
    updateEnvelopeParameters();
}

void FmOperator::setDecay(float& decay)
{
    m_decay = decay;
    updateEnvelopeParameters();
}

void FmOperator::setSustain(float &sustain)
{
    m_sustain = sustain;
    updateEnvelopeParameters();
}

void FmOperator::setRelease(float &release)
{
    m_release = release;
    updateEnvelopeParameters();
}

float FmOperator::getAttack()
{
    return m_attack;
}

float FmOperator::getDecay()
{
    return m_decay;
}

float FmOperator::getSustain()
{
    return m_sustain;
}

float FmOperator::getRelease()
{
    return m_release;
}

void FmOperator::updateEnvelopeParameters()
{
    m_envelope.setParameters(juce::ADSR::Parameters(m_attack, m_decay, m_sustain, m_release));
}

juce::ADSR* FmOperator::getEnvelope()
{
    return &m_envelope;
}
