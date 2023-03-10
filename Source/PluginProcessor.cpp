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
                     std::make_unique<juce::AudioParameterFloat>("op01AA", "Operator01 Attack", 0.001f, 10.0f, m_operator01AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>("op01AD", "Operator01 Decay", 0.001, 10.0f, m_operator01AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>("op01AS", "Operator01 Sustain", 0.001f, 1.0f, m_operator01AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>("op01AR", "Operator01 Release", 0.001f, 10.0f, m_operator01AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>("operator02Frequency", "Operator02 Frequency", m_frequencyRange, m_operator02Frequency),
                     std::make_unique<juce::AudioParameterFloat>("operator02Depth", "Operator02 Depth", 0.0f, 20000.0f, m_operator02Depth),
                     std::make_unique<juce::AudioParameterFloat>("op02AA", "Operator02 Attack", 0.001f, 10.0f, m_operator02AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>("op02AD", "Operator02 Decay", 0.001, 10.0f, m_operator02AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>("op02AS", "Operator02 Sustain", 0.001f, 1.0f, m_operator02AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>("op02AR", "Operator02 Release", 0.001f, 10.0f, m_operator02AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>("operator03Frequency", "Operator03 Frequency", m_frequencyRange, m_operator03Frequency),
                     std::make_unique<juce::AudioParameterFloat>("operator03Depth", "Operator03 Depth", 0.0f, 20000.0f, m_operator03Depth),
                     std::make_unique<juce::AudioParameterFloat>("op03AA", "Operator03 Attack", 0.01f, 10.0f, m_operator03AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>("op03AD", "Operator03 Decay", 0.01, 10.0f, m_operator03AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>("op03AS", "Operator03 Sustain", 0.01f, 1.0f, m_operator03AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>("op03AR", "Operator03 Release", 0.01f, 10.0f, m_operator03AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>("operator04Frequency", "Operator04 Frequency", m_frequencyRange, m_operator04Frequency),
                     std::make_unique<juce::AudioParameterFloat>("operator04Depth", "Operator04 Depth", 0.0f, 20000.0f, m_operator04Depth),
                    std::make_unique<juce::AudioParameterFloat>("op04AA", "Operator04 Attack", 0.01f, 10.0f, m_operator04AmpAttack),
                    std::make_unique<juce::AudioParameterFloat>("op04AD", "Operator04 Decay", 0.01, 10.0f, m_operator04AmpDecay),
                    std::make_unique<juce::AudioParameterFloat>("op04AS", "Operator04 Sustain", 0.01f, 1.0f, m_operator04AmpSustain),
                    std::make_unique<juce::AudioParameterFloat>("op04AR", "Operator04 Release", 0.01f, 10.0f, m_operator04AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterBool>("filterBypass", "Filter Bypass", 1),
                     std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 50.0f, 20000.0f, 20000.0f)
        }),
        m_synthesiser(4)
{
    m_valueTree.addParameterListener("playing", this);
    m_valueTree.addParameterListener("nAttack", this);
    m_valueTree.addParameterListener("nDecay", this);
    m_valueTree.addParameterListener("nSustain", this);
    m_valueTree.addParameterListener("nRelease", this);
    
    m_valueTree.addParameterListener("mode", this);
    
    m_valueTree.addParameterListener("algorithm", this);
    
    m_valueTree.addParameterListener("operator01Frequency", this);
    m_valueTree.addParameterListener("operator02Frequency", this);
    m_valueTree.addParameterListener("operator03Frequency", this);
    m_valueTree.addParameterListener("operator04Frequency", this);
    
    m_valueTree.addParameterListener("operator02Depth", this);
    m_valueTree.addParameterListener("operator03Depth", this);
    m_valueTree.addParameterListener("operator04Depth", this);
    
    m_valueTree.addParameterListener("op01AA", this);
    m_valueTree.addParameterListener("op01AD", this);
    m_valueTree.addParameterListener("op01AS", this);
    m_valueTree.addParameterListener("op01AR", this);
    
    m_valueTree.addParameterListener("op02AA", this);
    m_valueTree.addParameterListener("op02AD", this);
    m_valueTree.addParameterListener("op02AS", this);
    m_valueTree.addParameterListener("op02AR", this);
    
    m_valueTree.addParameterListener("op03AA", this);
    m_valueTree.addParameterListener("op03AD", this);
    m_valueTree.addParameterListener("op03AS", this);
    m_valueTree.addParameterListener("op03AR", this);
    
    m_valueTree.addParameterListener("op04AA", this);
    m_valueTree.addParameterListener("op04AD", this);
    m_valueTree.addParameterListener("op04AS", this);
    m_valueTree.addParameterListener("op04AR", this);
    
    m_valueTree.addParameterListener("gain", this);
    m_valueTree.addParameterListener("filterBypass", this);
    m_valueTree.addParameterListener("cutoff", this);
    
    m_synthesiser.getOperator(0)->setFrequency(m_operator01Frequency);
    m_synthesiser.getOperator(1)->setFrequency(m_operator02Frequency);
    m_synthesiser.getOperator(2)->setFrequency(m_operator03Frequency);
    m_synthesiser.getOperator(3)->setFrequency(m_operator04Frequency);
    
    m_synthesiser.getOperator(1)->setModDepth(m_operator02Depth);
    m_synthesiser.getOperator(2)->setModDepth(m_operator03Depth);
    m_synthesiser.getOperator(3)->setModDepth(m_operator04Depth);
    
    
    m_filter.setEnabled(false);
    m_filter.setMode(juce::dsp::LadderFilterMode::LPF12);
    m_filter.setCutoffFrequencyHz(20000.0f);
    m_filter.setResonance(0);
    m_filter.setDrive(1);
    
    updateNoiseEnvelopeParameters();
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

void UnitySynthesiserAudioProcessor::updateNoiseEnvelopeParameters()
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
        updateNoiseEnvelopeParameters();
        
    }
    else if (parameterID == "nDecay")
    {
        m_noiseEnvelopeD = newValue;
        updateNoiseEnvelopeParameters();
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
        updateNoiseEnvelopeParameters();
    }
    else if (parameterID == "nRelease")
    {
        m_noiseEnvelopeR = newValue;
        updateNoiseEnvelopeParameters();
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
    else if (parameterID == "operator01Frequency")
    {
        m_synthesiser.getOperator(0)->setFrequency(newValue);
    }
    else if (parameterID == "operator02Frequency")
    {
        m_synthesiser.getOperator(1)->setFrequency(newValue);
    }
    else if (parameterID == "operator02Depth")
    {
        m_synthesiser.getOperator(1)->setModDepth(newValue);
    }
    else if (parameterID == "operator03Frequency")
    {
        m_synthesiser.getOperator(2)->setFrequency(newValue);
    }
    else if (parameterID == "operator03Depth")
    {
        m_synthesiser.getOperator(2)->setModDepth(newValue);
    }
    else if (parameterID == "operator04Frequency")
    {
        m_synthesiser.getOperator(3)->setFrequency(newValue);
    }
    else if (parameterID == "operator04Depth")
    {
        m_synthesiser.getOperator(3)->setModDepth(newValue);
    }
    else if (parameterID == "op01AA")
    {
        m_synthesiser.getOperator(0)->setAmpAttack(newValue);
    }
    else if (parameterID == "op01AD")
    {
        m_synthesiser.getOperator(0)->setAmpDecay(newValue);
    }
    else if (parameterID == "op01AS")
    {
        m_synthesiser.getOperator(0)->setAmpSustain(newValue);
    }
    else if (parameterID == "op01AR")
    {
        m_synthesiser.getOperator(0)->setAmpRelease(newValue);
    }
    else if (parameterID == "op02AA")
    {
        m_synthesiser.getOperator(1)->setAmpAttack(newValue);
    }
    else if (parameterID == "op02AD")
    {
        m_synthesiser.getOperator(1)->setAmpDecay(newValue);
    }
    else if (parameterID == "op02AS")
    {
        m_synthesiser.getOperator(1)->setAmpSustain(newValue);
    }
    else if (parameterID == "op02AR")
    {
        m_synthesiser.getOperator(1)->setAmpRelease(newValue);
    }
    else if (parameterID == "op03AA")
    {
        m_synthesiser.getOperator(2)->setAmpAttack(newValue);
    }
    else if (parameterID == "op03AD")
    {
        m_synthesiser.getOperator(2)->setAmpDecay(newValue);
    }
    else if (parameterID == "op03AS")
    {
        m_synthesiser.getOperator(2)->setAmpSustain(newValue);
    }
    else if (parameterID == "op03AR")
    {
        m_synthesiser.getOperator(2)->setAmpRelease(newValue);
    }
    else if (parameterID == "op04AA")
    {
        m_synthesiser.getOperator(3)->setAmpAttack(newValue);
    }
    else if (parameterID == "op04AD")
    {
        m_synthesiser.getOperator(3)->setAmpDecay(newValue);
    }
    else if (parameterID == "op04AS")
    {
        m_synthesiser.getOperator(3)->setAmpSustain(newValue);
    }
    else if (parameterID == "op04AR")
    {
        m_synthesiser.getOperator(3)->setAmpRelease(newValue);
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

