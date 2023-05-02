/*
  ==============================================================================

    FmOperator.h
    Created: 21 Feb 2023 1:47:15pm
    Author:  Antti

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

///This FM operator class contains a sinewave oscillator, a pitch envelope and a volume envelope
class FmOperator
{
  
public:
    
    ///The constructor
    FmOperator(float baseFrequency);
    ///This function updates the phase angle of the sinewave oscillator
    void updateAngleDelta();
    ///This functions prepares the operator for playback by setting a correct samplerate
    void prepareToPlay(double sampleRate);
    ///This function returns the next sample of the operator based on the phase angle and the status of the envelopes
    float getNextSample();
    ///Setter for the m_baseFrequency member
    void setFrequency(float frequency);
    ///Setter for the m_modulationSample member
    void setModSample(float sample);
    ///Setter for the m_modulationDepth member
    void setModDepth(float depth);
    ///Setter for the m_ampAttack member
    void setAmpAttack(float attack);
    ///Setter for the m_ampDecay member
    void setAmpDecay(float decay);
    ///Setter for the m_ampSustain member
    void setAmpSustain(float sustain);
    ///Setter for the m_ampRelease member
    void setAmpRelease(float release);
    ///Getter for the m_ampAttack member
    float getAmpAttack();
    ///Getter for the m_ampDecay member
    float getAmpDecay();
    ///Getter for the m_ampSustain member
    float getAmpSustain();
    ///Getter for the m_ampRelease member
    float getAmpRelease();
    ///This function updates the parameters for the m_ampEnvelope member
    void updateAmpEnvelopeParameters();
    ///This function returns a pointer to the m_ampEnvelope member
    juce::ADSR* getAmpEnvelope();
    ///Setter for the m_freqAttack member
    void setFreqAttack(float attack);
    ///Setter for the m_freqDecay member
    void setFreqDecay(float decay);
    ///Setter for the m_freqSustain member
    void setFreqSustain(float sustain);
    ///Setter for the m_freqRelease member
    void setFreqRelease(float release);
    ///Getter for the m_freqAttack member
    float getFreqAttack();
    ///Getter for the m_freqDecay member
    float getFreqDecay();
    ///Getter for the m_freqSustain member
    float getFreqSustain();
    ///Getter for the m_freqSustain member
    float getFreqRelease();
    ///This function updates the parameters for the m_freqEnvelope member
    void updateFreqEnvelopeParameters();
    ///This function returns a pointer to the m_freqEnvelope member
    juce::ADSR* getFreqEnvelope();
    
private:
    ///This member is the current angle of the sinewave oscillator
    double m_currentAngle { 0 };
    ///This member is the amount by which the angle of the sinewave oscillator needs to change next sample
    double m_angleDelta { 0 };
    ///This member is the  sample rate
    double m_sampleRate { 0 };
    ///This member is the frequency of the operator after it has been modulated by m_modulationSample and m_freqEnvelope
    double m_frequency;
    ///This member is the base frequency of the operator. It needs to be Atomic to be threadsafe as the user interacts with this member
    juce::Atomic<double> m_baseFrequency;
    ///This member is the modulation sample used to modulate the frequency of this operator
    float m_modulationSample { 0 };
    ///This member is the modulation depth of the operator. It needs to be Atomic to be threadsafe as the user interacts with this member
    juce::Atomic<float> m_modulationDepth { 1.0f };
    ///This member is the attack parameter of the m_ampEnvelope member
    float m_ampAttack { 0 };
    ///This member is the decay parameter of the m_ampEnvelope member
    float m_ampDecay { 0 };
    ///This member is the sustain parameter of the m_ampEnvelope member
    float m_ampSustain { 0 };
    ///This member is the release parameter of the m_ampEnvelope member
    float m_ampRelease { 0 };
    ///This member is the attack parameter of the m_freqEnvelope member
    float m_freqAttack {0 };
    ///This member is the decay parameter of the m_freqEnvelope member
    float m_freqDecay { 0 };
    ///This member is the sustain parameter of the m_freqEnvelope member
    float m_freqSustain { 0 };
    ///This member is the release parameter of the m_freqEnvelope member
    float m_freqRelease { 0 };
    ///This member is the envelope used to control the amplitude of this operator
    juce::ADSR m_ampEnvelope;
    ///This member is the envelope used to control the frequency of the operator
    juce::ADSR m_freqEnvelope;
    
};
