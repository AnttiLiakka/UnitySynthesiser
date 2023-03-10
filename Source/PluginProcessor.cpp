/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
UnitySynthesiserAudioProcessor::UnitySynthesiserAudioProcessor()
     :  AudioProcessor (BusesProperties()
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
        m_synthesiser(m_valueTree, 4),
        m_frequencyRange(juce::NormalisableRange<float>(0, 14080, 220)),
        m_valueTree(*this, nullptr, "Parameters",
                    {std::make_unique<juce::AudioParameterBool>("playing", "Playing", 0),
                     std::make_unique<juce::AudioParameterChoice>("mode", "Mode", juce::StringArray{"FM", "Noise"}, 0),
                     std::make_unique<juce::AudioParameterFloat>("nAttack", "Noise Attack", 0.1f, 10.0f, m_noiseEnvelopeA),
                     std::make_unique<juce::AudioParameterFloat>("nDecay", "Noise Decay", 0.1, 10.0f, m_noiseEnvelopeD),
                     std::make_unique<juce::AudioParameterFloat>("nSustain", "Noise Sustain", 0.1f, 1.0f, m_noiseEnvelopeS),
                     std::make_unique<juce::AudioParameterFloat>("nRelease", "Noise Release", 0.1f, 10.0f, m_noiseEnvelopeR),
                     std::make_unique<juce::AudioParameterInt>("algorithm", "FM Algorithm", 1, 4, 1),
                     std::make_unique<juce::AudioParameterFloat>("operator01Frequency", "Operator01 Frequency", m_frequencyRange, m_operator01Frequency),
                     std::make_unique<juce::AudioParameterFloat>("op01A", "Operator01 Attack", 0.001f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("op01D", "Operator01 Decay", 0.001, 10.0f, 0.1),
                     std::make_unique<juce::AudioParameterFloat>("op01S", "Operator01 Sustain", 0.001f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("op01R", "Operator01 Release", 0.001f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("operator02Frequency", "Operator02 Frequency", m_frequencyRange, m_operator02Frequency),
                     std::make_unique<juce::AudioParameterFloat>("operator02Depth", "Operator02 Depth", 0.0f, 20000.0f, m_operator02Depth),
                     std::make_unique<juce::AudioParameterFloat>("op02A", "Operator02 Attack", 0.001f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("op02D", "Operator02 Decay", 0.001, 10.0f, 0.1),
                     std::make_unique<juce::AudioParameterFloat>("op02S", "Operator02 Sustain", 0.001f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("op02R", "Operator02 Release", 0.001f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("operator03Frequency", "Operator03 Frequency", m_frequencyRange, m_operator03Frequency),
                     std::make_unique<juce::AudioParameterFloat>("operator03Depth", "Operator03 Depth", 0.0f, 20000.0f, m_operator03Depth),
                     std::make_unique<juce::AudioParameterFloat>("op03A", "Operator03 Attack", 0.01f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("op03D", "Operator03 Decay", 0.01, 10.0f, 0.1),
                     std::make_unique<juce::AudioParameterFloat>("op03S", "Operator03 Sustain", 0.01f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterFloat>("op03R", "Operator03 Release", 0.01f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("operator04Frequency", "Operator04 Frequency", m_frequencyRange, m_operator04Frequency),
                     std::make_unique<juce::AudioParameterFloat>("operator04Depth", "Operator04 Depth", 0.0f, 20000.0f, m_operator04Depth),
                    std::make_unique<juce::AudioParameterFloat>("op04A", "Operator04 Attack", 0.01f, 10.0f, 0.1f),
                    std::make_unique<juce::AudioParameterFloat>("op04D", "Operator04 Decay", 0.01, 10.0f, 0.1),
                    std::make_unique<juce::AudioParameterFloat>("op04S", "Operator04 Sustain", 0.01f, 1.0f, 0.5f),
                    std::make_unique<juce::AudioParameterFloat>("op04R", "Operator04 Release", 0.01f, 10.0f, 0.1f),
                     std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterBool>("filterBypass", "Filter Bypass", 1),
                     std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 50.0f, 20000.0f, 20000.0f)
        })
{
    m_valueTree.addParameterListener("playing", this);
    m_valueTree.addParameterListener("nAttack", this);
    m_valueTree.addParameterListener("nDecay", this);
    m_valueTree.addParameterListener("nSustain", this);
    m_valueTree.addParameterListener("nRelease", this);
    
    m_valueTree.addParameterListener("mode", this);
    
    m_valueTree.addParameterListener("algorithm", this);
    
    m_valueTree.addParameterListener("gain", this);
    m_valueTree.addParameterListener("filterBypass", this);
    m_valueTree.addParameterListener("cutoff", this);
    
    m_filter.setEnabled(false);
    m_filter.setMode(juce::dsp::LadderFilterMode::LPF12);
    m_filter.setCutoffFrequencyHz(20000.0f);
    m_filter.setResonance(0);
    m_filter.setDrive(1);
    
    updateEnvelopeParameters();
}

UnitySynthesiserAudioProcessor::~UnitySynthesiserAudioProcessor()
{
}

//==============================================================================
const juce::String UnitySynthesiserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UnitySynthesiserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UnitySynthesiserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UnitySynthesiserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UnitySynthesiserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UnitySynthesiserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UnitySynthesiserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UnitySynthesiserAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String UnitySynthesiserAudioProcessor::getProgramName (int index)
{
    return {};
}

void UnitySynthesiserAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void UnitySynthesiserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_synthesiser.prepareToPlay(sampleRate);
    
    m_spec.maximumBlockSize = samplesPerBlock;
    m_spec.sampleRate = sampleRate;
    m_spec.numChannels = getTotalNumOutputChannels();
    
    m_gain.prepare(m_spec);
    m_filter.prepare(m_spec);
    m_noiseEnvelope.setSampleRate(sampleRate);
    
    m_gain.setGainLinear(0.5f);
     
}

void UnitySynthesiserAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UnitySynthesiserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void UnitySynthesiserAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    
    if (!m_playNoise)
    {
        m_synthesiser.processNextBlock(audioBlock);
    }
    else
    {
        auto* leftChannel = audioBlock.getChannelPointer(0);
        auto* rightChannel = audioBlock.getChannelPointer(1);
        
        for (int n = 0; n < audioBlock.getNumSamples(); ++n)
        {
            auto sample = m_random.nextFloat() * m_noiseEnvelope.getNextSample();
            
            leftChannel[n] = sample;
            rightChannel[n] = sample;
        }
    }
    m_filter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    m_gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

//==============================================================================
bool UnitySynthesiserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* UnitySynthesiserAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void UnitySynthesiserAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UnitySynthesiserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UnitySynthesiserAudioProcessor();
}

void UnitySynthesiserAudioProcessor::updateEnvelopeParameters()
{
    m_noiseEnvelope.setParameters(juce::ADSR::Parameters(m_noiseEnvelopeA, m_noiseEnvelopeD, m_noiseEnvelopeS, m_noiseEnvelopeR));
}

void UnitySynthesiserAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "playing")
    {
        if(newValue == 1)
        {
            m_noiseEnvelope.noteOn();
            m_synthesiser.noteOn();
        }
        else
        {
            m_noiseEnvelope.noteOff();
            m_synthesiser.noteOff();
        }
    }
    else if (parameterID == "nAttack")
    {
        m_noiseEnvelopeA = newValue;
        updateEnvelopeParameters();
        
    }
    else if (parameterID == "nDecay")
    {
        m_noiseEnvelopeD = newValue;
        updateEnvelopeParameters();
    }
    else if (parameterID == "nSustain")
    {
        if(newValue > 1)
        {
            m_noiseEnvelopeS = 1.0f;
        }
        else
        {
            m_noiseEnvelopeS = newValue;
        }
        updateEnvelopeParameters();
    }
    else if (parameterID == "nRelease")
    {
        m_noiseEnvelopeR = newValue;
        updateEnvelopeParameters();
    }
    else if (parameterID == "mode")
    {
        if (newValue == 0)
        {
            m_playNoise = false;
        }
        else if (newValue == 1)
        {
            m_playNoise = true;
        }
    }
    else if (parameterID == "algorithm")
    {
        m_synthesiser.changeAlgorithm((int) newValue);
    }
    else if (parameterID == "gain")
    {
        if (newValue > 1.0f)
        {
            m_gain.setGainLinear(1.0f);
        }
        else
        {
            m_gain.setGainLinear(newValue);
        }
    }
    else if (parameterID == "filterBypass")
    {
        if (newValue == 1)
        {
            m_filter.setEnabled(false);
        }
        else
        {
            m_filter.setEnabled(true);
        }
    }
    else if (parameterID == "cutoff")
    {
        m_filter.setCutoffFrequencyHz(newValue);
    }
    else jassertfalse; // You probably have a typo somewhere
}

