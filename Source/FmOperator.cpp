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
    updateAmpEnvelopeParameters();
    
    m_freqEnvelope.setSampleRate(sampleRate);
    updateFreqEnvelopeParameters();
}

float FmOperator::getNextSample()
{
    auto currentSample = (float) std::sin(m_currentAngle) * (m_modulationDepth * m_ampEnvelope.getNextSample());
    m_currentAngle += m_angleDelta;
    
    m_frequency = m_baseFrequency + (m_modulationSample * m_freqEnvelope.getNextSample());
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
    updateAmpEnvelopeParameters();
}

void FmOperator::setModDepth(float depth)
{
    m_modulationDepth = depth;
    updateAmpEnvelopeParameters();
}

void FmOperator::setAmpAttack(float attack)
{
    m_ampAttack = attack;
    updateAmpEnvelopeParameters();
}

void FmOperator::setAmpDecay(float decay)
{
    m_ampDecay = decay;
    updateAmpEnvelopeParameters();
}

void FmOperator::setAmpSustain(float sustain)
{
    m_ampSustain = sustain;
    updateAmpEnvelopeParameters();
}

void FmOperator::setAmpRelease(float release)
{
    m_ampRelease = release;
    updateAmpEnvelopeParameters();
}

float FmOperator::getAmpAttack()
{
    return m_ampAttack;
}

float FmOperator::getAmpDecay()
{
    return m_ampDecay;
}

float FmOperator::getAmpSustain()
{
    return m_ampSustain;
}

float FmOperator::getAmpRelease()
{
    return m_ampRelease;
}

void FmOperator::updateAmpEnvelopeParameters()
{
    m_ampEnvelope.setParameters(juce::ADSR::Parameters(m_ampAttack, m_ampDecay, m_ampSustain, m_ampRelease));
}

juce::ADSR* FmOperator::getAmpEnvelope()
{
    return &m_ampEnvelope;
}

void FmOperator::setFreqAttack(float attack)
{
    m_freqAttack = attack;
    updateAmpEnvelopeParameters();
}

void FmOperator::setFreqDecay(float decay)
{
    m_freqDecay = decay;
    updateAmpEnvelopeParameters();
}

void FmOperator::setFreqSustain(float sustain)
{
    m_freqSustain = sustain;
    updateAmpEnvelopeParameters();
}

void FmOperator::setFreqRelease(float release)
{
    m_freqRelease = release;
    updateAmpEnvelopeParameters();
}

float FmOperator::getFreqAttack()
{
    return m_freqAttack;
}

float FmOperator::getFreqDecay()
{
    return m_freqDecay;
}

float FmOperator::getFreqSustain()
{
    return m_freqSustain;
}

float FmOperator::getFreqRelease()
{
    return m_freqRelease;
}

void FmOperator::updateFreqEnvelopeParameters()
{
    m_freqEnvelope.setParameters(juce::ADSR::Parameters(m_freqAttack, m_freqDecay, m_freqSustain, m_freqRelease));
}

juce::ADSR* FmOperator::getFreqEnvelope()
{
    return &m_freqEnvelope;
}
