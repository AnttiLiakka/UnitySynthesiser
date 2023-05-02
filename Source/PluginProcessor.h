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
///This class is the main component of the pluging, it inherits from the juce::AudioProcessor class
class UnitySynthesiserAudioProcessor  : public juce::AudioProcessor,
                            public juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
    
public:
    //==============================================================================
    ///The default constructor
    UnitySynthesiserAudioProcessor();
    
    /// The destructor
    ~UnitySynthesiserAudioProcessor() override;

    //==============================================================================
    ///Pure virtual function inherited from juce::AudioProcessor. It is overridden to set the correct sample rate to the members that need it
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    ///Pure virtual function inherited from juce::AudioProcessor.
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    ///Pure birtual function inherited from juce::AudioProcessor
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    ///Pure virtual functon inherited from juce::AudioProcessor. It is overridden to process the audiobuffer via the m_synthesiser member
    ///or to fill the buffer with noise, depending what the user has selected
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    ///Pure virtual function inherited from juce::AudioProcessor
    juce::AudioProcessorEditor* createEditor() override;
    ///Pure virtual function inherited from juce::AudioProcessor
    bool hasEditor() const override;

    //==============================================================================
    ///Pure virtual function inherited from juce::AudioProcessor
    const juce::String getName() const override;
    
    ///Pure virtual function inherited from juce::AudioProcessor
    bool acceptsMidi() const override;
    ///Pure virtual function inherited from juce::AudioProcessor
    bool producesMidi() const override;
    ///Pure virtual function inherited from juce::AudioProcessor
    bool isMidiEffect() const override;
    ///Pure virtual function inherited from juce::AudioProcessor
    double getTailLengthSeconds() const override;

    //==============================================================================
    ///Pure virtual function inherited from juce::AudioProcessor
    int getNumPrograms() override;
    ///Pure virtual function inherited from juce::AudioProcessor
    int getCurrentProgram() override;
    ///Pure virtual function inherited from juce::AudioProcessor
    void setCurrentProgram (int index) override;
    ///Pure virtual function inherited from juce::AudioProcessor
    const juce::String getProgramName (int index) override;
    ///Pure virtual function inherited from juce::AudioProcessor
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    ///Pure virtual function inherited from juce::AudioProcessor
    void getStateInformation (juce::MemoryBlock& destData) override;
    ///Pure virtual function inherited from juce::AudioProcessor
    void setStateInformation (const void* data, int sizeInBytes) override;
    ///This function updates the parameters for the m_noiseEnvelope member
    void updateNoiseEnvelopeParameters();
    ///This function changes the m_preset member
    void changePreset(float preset);
    ///Pure virtual function inherited from juce::AudioProcessorValueTreeState::Listener
    ///It is overridden to listen to the changes in the m_valueTree and update the plugin parameters accordingly
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    ///This member is the frequency range for the frequency parameters
    juce::NormalisableRange<float> m_frequencyRange;
    ///This member is the value range for the attac,decay and release envelope parameters
    juce::NormalisableRange<float> m_envelopeRange;
    ///This member is used to generate random values for the noise functionality
    juce::Random m_random;
    ///This member is the procces spec for the dps functionalities
    juce::dsp::ProcessSpec m_spec;
    ///This member is the envelope controlling the noise amplitude
    juce::ADSR m_noiseEnvelope;
    ///This member is the ladder filters used to apply a low pass filtering to the audioblock
    juce::dsp::LadderFilter<float> m_filter;
    ///This member is the gain used to apply gain to the audioblock
    juce::dsp::Gain<float> m_gain;
    
    ///This member states whether the plugin should be processing audio
    bool m_playing { false };
    ///This member states whether the plugin should use the noise generation functionality or the m_synthesiser
    bool m_playNoise { false };
    ///This member is the preset currently used by the plugin
    int m_preset { 0 };
    ///This member is the attack parameters for the m_noiseEnvelope
    float m_noiseEnvelopeA { 1.0 };
    ///This member is the decay parameter for the m_noiseEnvelope
    float m_noiseEnvelopeD { 1.0 };
    ///This member is the sustain parameter for the m_noiseEnvelope
    float m_noiseEnvelopeS { 1.0 };
    ///This member is the release parameter for the m_noiseEnvelope
    float m_noiseEnvelopeR { 1.0 };
    ///These members are the default parameters for the operator01 of the m_synthesiser
    float m_operator01Frequency { 220.0 },
          m_operator01AmpAttack { 1.0 },
          m_operator01AmpDecay { 1.0 },
          m_operator01AmpSustain { 1.0 },
          m_operator01AmpRelease { 1.0 },
          m_operator01FreqAttack { 10.0 },
          m_operator01FreqDecay { 1.0 },
          m_operator01FreqSustain { 1.0 },
          m_operator01FreqRelease { 10.0 };
    ///These members are the default parameters for the operator02 of the m_synthesiser
    float m_operator02Frequency { 440.0 },
          m_operator02Depth { 1000.0 },
          m_operator02AmpAttack { 10.0 },
          m_operator02AmpDecay { 10.0 },
          m_operator02AmpSustain { 1.0 },
          m_operator02AmpRelease { 10.0 },
          m_operator02FreqAttack { 10.0 },
          m_operator02FreqDecay { 10.0 },
          m_operator02FreqSustain { 1.0 },
          m_operator02FreqRelease { 10.0 };
    ///These members are the default parameters for the operator03 of the m_synthesiser
    float m_operator03Frequency { 880.0 },
          m_operator03Depth { 1000.0 },
          m_operator03AmpAttack { 10.0 },
          m_operator03AmpDecay { 10.0 },
          m_operator03AmpSustain { 1.0 },
          m_operator03AmpRelease { 10.0 },
          m_operator03FreqAttack { 10.0 },
          m_operator03FreqDecay { 10.0 },
          m_operator03FreqSustain { 1.0 },
          m_operator03FreqRelease { 10.0 };
    ///These members are the default parameters for the operator03 of the m_synthesiser
    float m_operator04Frequency { 1760.0 },
          m_operator04Depth { 1000.0 },
          m_operator04AmpAttack { 10.0 },
          m_operator04AmpDecay { 10.0 },
          m_operator04AmpSustain { 1.0 },
          m_operator04AmpRelease { 10.0 },
          m_operator04FreqAttack { 10.0 },
          m_operator04FreqDecay { 10.0 },
          m_operator04FreqSustain { 1.0 },
          m_operator04FreqRelease { 10.0 };
    ///These members are the static strings used to identify a parameter in the m_valueTree
    static juce::String m_presetID,
                        m_playingID,
                        m_modeID,
                        m_nAttackID,
                        m_nDecayID,
                        m_nSustainID,
                        m_nReleaseID,
                        m_algorithmID,
    
                        m_op01FreqID,
                        m_op01AmpAttID,
                        m_op01AmpDecID,
                        m_op01AmpSusID,
                        m_op01AmpRelID,
                        m_op01FreqAttID,
                        m_op01FreqDecID,
                        m_op01FreqSusID,
                        m_op01FreqRelID,
    
                        m_op02FreqID,
                        m_op02DepthID,
                        m_op02AmpAttID,
                        m_op02AmpDecID,
                        m_op02AmpSusID,
                        m_op02AmpRelID,
                        m_op02FreqAttID,
                        m_op02FreqDecID,
                        m_op02FreqSusID,
                        m_op02FreqRelID,
    
                        m_op03FreqID,
                        m_op03DepthID,
                        m_op03AmpAttID,
                        m_op03AmpDecID,
                        m_op03AmpSusID,
                        m_op03AmpRelID,
                        m_op03FreqAttID,
                        m_op03FreqDecID,
                        m_op03FreqSusID,
                        m_op03FreqRelID,
    
                        m_op04FreqID,
                        m_op04DepthID,
                        m_op04AmpAttID,
                        m_op04AmpDecID,
                        m_op04AmpSusID,
                        m_op04AmpRelID,
                        m_op04FreqAttID,
                        m_op04FreqDecID,
                        m_op04FreqSusID,
                        m_op04FreqRelID,
    
                        m_gainID,
                        m_filterBypassID,
                        m_filterCutoffID;
    ///This member is the valuetree in which the parameters of this plugin are stored
    juce::AudioProcessorValueTreeState m_valueTree;
    ///This member is the FM synthesiser
    FmSynthesiser m_synthesiser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitySynthesiserAudioProcessor)
};
