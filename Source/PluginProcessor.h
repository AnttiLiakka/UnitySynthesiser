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
    
    bool m_playing = false;
    bool m_playNoise = false;
    
    float m_envelopeAttack = 0.1f;
    float m_envelopeDecay = 0.1f;
    float m_envelopeSustain = 1.0f;
    float m_envelopeRelease = 0.1f;
    
    float m_operator01Frequency = 440.0;
    
    float m_operator02Frequency = 10.0f;
    float m_operator02Depth = 1000.0f;
    
    juce::Random random;
    
    juce::dsp::ProcessSpec m_spec;
    
    juce::ADSR m_adsr;
    juce::dsp::LadderFilter<float> m_filter;
    juce::dsp::Gain<float> m_gain;
    
    juce::AudioProcessorValueTreeState m_valueTree;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySynthesiserAudioProcessor)
};
