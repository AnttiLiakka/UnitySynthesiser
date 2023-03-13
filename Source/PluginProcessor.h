/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FmSynthesiser.h"
#include <chrono>
#include <thread>
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
    
    void changePreset(float preset);
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    
    juce::NormalisableRange<float> m_frequencyRange;
    juce::NormalisableRange<float> m_envelopeRange;
    juce::Random m_random;
    juce::dsp::ProcessSpec m_spec;
    juce::ADSR m_noiseEnvelope;
    juce::dsp::LadderFilter<float> m_filter;
    juce::dsp::Gain<float> m_gain;
    
    bool m_playing = false;
    bool m_playNoise = false;
    
    int m_preset = 0;
    
    float m_noiseEnvelopeA = 1.0f;
    float m_noiseEnvelopeD = 1.0f;
    float m_noiseEnvelopeS = 1.0f;
    float m_noiseEnvelopeR = 1.0f;
    
    float m_operator01Frequency = 220.0f;
    float m_operator01AmpAttack = 1.0;
    float m_operator01AmpDecay = 1.0;
    float m_operator01AmpSustain = 1.0;
    float m_operator01AmpRelease = 1.0;
    float m_operator01FreqAttack = 10.0;
    float m_operator01FreqDecay = 1.0;
    float m_operator01FreqSustain = 1.0;
    float m_operator01FreqRelease = 10.0;
    
    float m_operator02Frequency = 440.0f;
    float m_operator02Depth = 1000.0f;
    float m_operator02AmpAttack = 10.0;
    float m_operator02AmpDecay = 10.0;
    float m_operator02AmpSustain = 1.0;
    float m_operator02AmpRelease = 10.0;
    float m_operator02FreqAttack = 10.0;
    float m_operator02FreqDecay = 10.0;
    float m_operator02FreqSustain = 1.0;
    float m_operator02FreqRelease = 10.0;
    
    float m_operator03Frequency = 880.0f;
    float m_operator03Depth = 1000.0f;
    float m_operator03AmpAttack = 10.0;
    float m_operator03AmpDecay = 10.0;
    float m_operator03AmpSustain = 1.0;
    float m_operator03AmpRelease = 10.0;
    float m_operator03FreqAttack = 10.0;
    float m_operator03FreqDecay = 10.0;
    float m_operator03FreqSustain = 1.0;
    float m_operator03FreqRelease = 10.0;
    
    float m_operator04Frequency = 1760.0f;
    float m_operator04Depth = 1000.0f;
    float m_operator04AmpAttack = 10.0;
    float m_operator04AmpDecay = 10.0;
    float m_operator04AmpSustain = 1.0;
    float m_operator04AmpRelease = 10.0;
    float m_operator04FreqAttack = 10.0;
    float m_operator04FreqDecay = 10.0;
    float m_operator04FreqSustain = 1.0;
    float m_operator04FreqRelease = 10.0;
    
    static juce::String m_presetID;
    static juce::String m_playingID;
    static juce::String m_modeID;
    static juce::String m_nAttackID;
    static juce::String m_nDecayID;
    static juce::String m_nSustainID;
    static juce::String m_nReleaseID;
    static juce::String m_algorithmID;
    
    static juce::String m_op01FreqID;
    static juce::String m_op01AmpAttID;
    static juce::String m_op01AmpDecID;
    static juce::String m_op01AmpSusID;
    static juce::String m_op01AmpRelID;
    static juce::String m_op01FreqAttID;
    static juce::String m_op01FreqDecID;
    static juce::String m_op01FreqSusID;
    static juce::String m_op01FreqRelID;
    
    static juce::String m_op02FreqID;
    static juce::String m_op02DepthID;
    static juce::String m_op02AmpAttID;
    static juce::String m_op02AmpDecID;
    static juce::String m_op02AmpSusID;
    static juce::String m_op02AmpRelID;
    static juce::String m_op02FreqAttID;
    static juce::String m_op02FreqDecID;
    static juce::String m_op02FreqSusID;
    static juce::String m_op02FreqRelID;
    
    static juce::String m_op03FreqID;
    static juce::String m_op03DepthID;
    static juce::String m_op03AmpAttID;
    static juce::String m_op03AmpDecID;
    static juce::String m_op03AmpSusID;
    static juce::String m_op03AmpRelID;
    static juce::String m_op03FreqAttID;
    static juce::String m_op03FreqDecID;
    static juce::String m_op03FreqSusID;
    static juce::String m_op03FreqRelID;
    
    static juce::String m_op04FreqID;
    static juce::String m_op04DepthID;
    static juce::String m_op04AmpAttID;
    static juce::String m_op04AmpDecID;
    static juce::String m_op04AmpSusID;
    static juce::String m_op04AmpRelID;
    static juce::String m_op04FreqAttID;
    static juce::String m_op04FreqDecID;
    static juce::String m_op04FreqSusID;
    static juce::String m_op04FreqRelID;
    
    static juce::String m_gainID;
    static juce::String m_filterBypassID;
    static juce::String m_filterCutoffID;

    juce::AudioProcessorValueTreeState m_valueTree;
    FmSynthesiser m_synthesiser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySynthesiserAudioProcessor)
};
