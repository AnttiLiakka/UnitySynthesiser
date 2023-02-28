/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FmSynthesiser.h"

//==============================================================================
/**
*/
class UnitySynthesiserAudioProcessor  : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
    
public:
    //==============================================================================
    UnitySynthesiserAudioProcessor();
    ~UnitySynthesiserAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void updateEnvelopeParameters();
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    
    FmSynthesiser m_synthesiser;
    
    juce::NormalisableRange<float> m_frequencyRange;
    juce::NormalisableRange<float> m_envelopeRange;
    juce::Random m_random;
    juce::dsp::ProcessSpec m_spec;
    juce::ADSR m_noiseEnvelope;
    juce::dsp::LadderFilter<float> m_filter;
    juce::dsp::Gain<float> m_gain;
    
    bool m_playing = false;
    bool m_playNoise = false;
    
    float m_noiseEnvelopeA = 0.1f;
    float m_noiseEnvelopeD = 0.1f;
    float m_noiseEnvelopeS = 1.0f;
    float m_noiseEnvelopeR = 10.0f;
    
    float m_operator01Frequency = 220.0f;
    
    float m_operator02Frequency = 440.0f;
    float m_operator02Depth = 2000.0f;
    
    float m_operator03Frequency = 880.0f;
    float m_operator03Depth = 2000.0f;
    
    float m_operator04Frequency = 1760.0f;
    float m_operator04Depth = 2000.0f;
    
    juce::AudioProcessorValueTreeState m_valueTree;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySynthesiserAudioProcessor)
};
