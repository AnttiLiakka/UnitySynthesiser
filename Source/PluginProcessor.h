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
    
    void updateNoiseEnvelopeParameters();
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    
    juce::NormalisableRange<float> m_frequencyRange;
    juce::Random m_random;
    juce::dsp::ProcessSpec m_spec;
    juce::ADSR m_noiseEnvelope;
    juce::dsp::LadderFilter<float> m_filter;
    juce::dsp::Gain<float> m_gain;
    
    bool m_playing = false;
    bool m_playNoise = false;
    
    float m_noiseEnvelopeA = 1.0f;
    float m_noiseEnvelopeD = 1.0f;
    float m_noiseEnvelopeS = 1.0f;
    float m_noiseEnvelopeR = 1.0f;
    
    float m_operator01Frequency = 220.0f;
    float m_operator01AmpAttack = 1.0;
    float m_operator01AmpDecay = 1.0;
    float m_operator01AmpSustain = 1.0;
    float m_operator01AmpRelease = 1.0;
    
    float m_operator02Frequency = 440.0f;
    float m_operator02Depth = 1000.0f;
    float m_operator02AmpAttack = 1.0;
    float m_operator02AmpDecay = 1.0;
    float m_operator02AmpSustain = 1.0;
    float m_operator02AmpRelease = 1.0;
    
    float m_operator03Frequency = 880.0f;
    float m_operator03Depth = 1000.0f;
    float m_operator03AmpAttack = 1.0;
    float m_operator03AmpDecay = 1.0;
    float m_operator03AmpSustain = 1.0;
    float m_operator03AmpRelease = 1.0;
    
    float m_operator04Frequency = 1760.0f;
    float m_operator04Depth = 1000.0f;
    float m_operator04AmpAttack = 1.0;
    float m_operator04AmpDecay = 1.0;
    float m_operator04AmpSustain = 1.0;
    float m_operator04AmpRelease = 1.0;

    juce::AudioProcessorValueTreeState m_valueTree;
    FmSynthesiser m_synthesiser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySynthesiserAudioProcessor)
};
