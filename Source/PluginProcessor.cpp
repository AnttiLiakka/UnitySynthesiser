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
        m_osc([](float x) {return std::sin(x);}, 510),
        m_fmOsc([](float x) {return std::sin(x);}, 510),
        m_valueTree(*this, nullptr, "Parameters",
                    {std::make_unique<juce::AudioParameterBool>("playing", "Playing", 0),
                     std::make_unique<juce::AudioParameterFloat>("attack", "Attack", 0.1f, 10.0f, m_envelopeAttack),
                     std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 0.1, 10.0f, m_envelopeDecay),
                     std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", 0.1f, 1.0f, m_envelopeSustain),
                     std::make_unique<juce::AudioParameterFloat>("release", "Release", 0.1f, 10.0f, m_envelopeRelease),
                     std::make_unique<juce::AudioParameterChoice>("waveform", "Waveform", juce::StringArray{"Sine", "Saw"}, 0),
                     std::make_unique<juce::AudioParameterFloat>("frequency", "Frequency", 10.0f, 20000.0f, m_oscFrequency),
                     std::make_unique<juce::AudioParameterFloat>("fmFrequency", "FM Frequency", 0.0f, 20000.0f, m_fmFrequency),
                     std::make_unique<juce::AudioParameterFloat>("fmDepth", "FM Dept", 0.0f, 1000.0f, m_fmDepth),
                     std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterBool>("filterBypass", "Filter Bypass", 1),
                     std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 50.0f, 20000.0f, 20000.0f)
        })
{
    m_valueTree.addParameterListener("playing", this);
    m_valueTree.addParameterListener("attack", this);
    m_valueTree.addParameterListener("decay", this);
    m_valueTree.addParameterListener("sustain", this);
    m_valueTree.addParameterListener("release", this);
    m_valueTree.addParameterListener("waveform", this);
    m_valueTree.addParameterListener("frequency", this);
    m_valueTree.addParameterListener("fmFrequency", this);
    m_valueTree.addParameterListener("fmDepth", this);
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
    
    m_spec.maximumBlockSize = samplesPerBlock;
    m_spec.sampleRate = sampleRate;
    m_spec.numChannels = getTotalNumOutputChannels();
    
    m_osc.prepare(m_spec);
    m_gain.prepare(m_spec);
    m_filter.prepare(m_spec);
    m_adsr.setSampleRate(sampleRate);
    
    m_fmOsc.prepare(m_spec);
    
    m_osc.setFrequency(440.0f);
    m_fmOsc.setFrequency(5.0f);
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
    
    //FM sample by sample processing
    for (int i = 0; i < audioBlock.getNumChannels(); ++i)
    {
        for (int s = 0; s < audioBlock.getNumSamples(); ++s)
        {
            m_fmModulation = m_fmOsc.processSample(audioBlock.getSample(i, s)) * m_fmDepth;
            auto freq = m_oscFrequency + m_fmModulation;
            m_osc.setFrequency(freq >= 0 ? freq : freq * -1.0f);
        }
    }
    
    m_osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    m_filter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    m_gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    m_adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());
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
    m_adsr.setParameters(juce::ADSR::Parameters(m_envelopeAttack, m_envelopeDecay, m_envelopeSustain, m_envelopeRelease));
}

void UnitySynthesiserAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "playing")
    {
        if(newValue == 1)
        {
            m_adsr.noteOn();
        }
        else
        {
            m_adsr.noteOff();
        }
    }
    else if (parameterID == "attack")
    {
        m_envelopeAttack = newValue;
        updateEnvelopeParameters();
        
    }
    else if (parameterID == "decay")
    {
        m_envelopeDecay = newValue;
        updateEnvelopeParameters();
    }
    else if (parameterID == "sustain")
    {
        if(newValue > 1)
        {
            m_envelopeSustain = 1.0f;
        }
        else
        {
            m_envelopeSustain = newValue;
        }
        updateEnvelopeParameters();
    }
    else if (parameterID == "release")
    {
        m_envelopeRelease = newValue;
        updateEnvelopeParameters();
    }
    else if (parameterID == "waveform")
    {
        if (newValue == 0)
        {
            m_osc.initialise([](float x) {return std::sin(x);}, 510);
        }
        else if (newValue == 1)
        {
            m_osc.initialise([](float x) {return x / juce::MathConstants<float>::pi;}, 510);
        }
    }
    else if (parameterID == "frequency")
    {
        m_osc.setFrequency(newValue + m_fmModulation);
        m_oscFrequency = newValue;
    }
    else if (parameterID == "fmFrequency")
    {
        m_fmOsc.setFrequency(newValue);
    }
    else if (parameterID == "fmDepth")
    {
        m_fmDepth = newValue;
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

