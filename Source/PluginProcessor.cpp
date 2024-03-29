/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//First the static strings need to be initialised
juce::String UnitySynthesiserAudioProcessor::m_presetID = juce::String("preset");
juce::String UnitySynthesiserAudioProcessor::m_playingID = juce::String("playing");
juce::String UnitySynthesiserAudioProcessor::m_modeID = juce::String("mode");
juce::String UnitySynthesiserAudioProcessor::m_nAttackID = juce::String("nAttack");
juce::String UnitySynthesiserAudioProcessor::m_nDecayID = juce::String("nDecay");
juce::String UnitySynthesiserAudioProcessor::m_nSustainID = juce::String("nSustain");
juce::String UnitySynthesiserAudioProcessor::m_nReleaseID = juce::String("nRelease");
juce::String UnitySynthesiserAudioProcessor::m_algorithmID = juce::String("algorithm");

juce::String UnitySynthesiserAudioProcessor::m_op01FreqID = juce::String("op01Freq");
juce::String UnitySynthesiserAudioProcessor::m_op01AmpAttID = juce::String("op01AmpAtt");
juce::String UnitySynthesiserAudioProcessor::m_op01AmpDecID = juce::String("op01AmpDec");
juce::String UnitySynthesiserAudioProcessor::m_op01AmpSusID = juce::String("op01AmpSus");
juce::String UnitySynthesiserAudioProcessor::m_op01AmpRelID = juce::String("op01AmpRel");
juce::String UnitySynthesiserAudioProcessor::m_op01FreqAttID = juce::String("op01FreqAtt");
juce::String UnitySynthesiserAudioProcessor::m_op01FreqDecID = juce::String("op01FreqDec");
juce::String UnitySynthesiserAudioProcessor::m_op01FreqSusID = juce::String("op01FreqSus");
juce::String UnitySynthesiserAudioProcessor::m_op01FreqRelID = juce::String("op01FreqRel");

juce::String UnitySynthesiserAudioProcessor::m_op02FreqID = juce::String("op02Freq");
juce::String UnitySynthesiserAudioProcessor::m_op02DepthID = juce::String("op02Depth");
juce::String UnitySynthesiserAudioProcessor::m_op02AmpAttID = juce::String("op02AmpAtt");
juce::String UnitySynthesiserAudioProcessor::m_op02AmpDecID = juce::String("op02AmpDec");
juce::String UnitySynthesiserAudioProcessor::m_op02AmpSusID = juce::String("op02AmpSus");
juce::String UnitySynthesiserAudioProcessor::m_op02AmpRelID = juce::String("op02AmpRel");
juce::String UnitySynthesiserAudioProcessor::m_op02FreqAttID = juce::String("op02FreqAtt");
juce::String UnitySynthesiserAudioProcessor::m_op02FreqDecID = juce::String("op02FreqDec");
juce::String UnitySynthesiserAudioProcessor::m_op02FreqSusID = juce::String("op02FreqSus");
juce::String UnitySynthesiserAudioProcessor::m_op02FreqRelID = juce::String("op02FreqRel");

juce::String UnitySynthesiserAudioProcessor::m_op03FreqID = juce::String("op03Freq");
juce::String UnitySynthesiserAudioProcessor::m_op03DepthID = juce::String("op03Depth");
juce::String UnitySynthesiserAudioProcessor::m_op03AmpAttID = juce::String("op03AmpAtt");
juce::String UnitySynthesiserAudioProcessor::m_op03AmpDecID = juce::String("op03AmpDec");
juce::String UnitySynthesiserAudioProcessor::m_op03AmpSusID = juce::String("op03AmpSus");
juce::String UnitySynthesiserAudioProcessor::m_op03AmpRelID = juce::String("op03AmpRel");
juce::String UnitySynthesiserAudioProcessor::m_op03FreqAttID = juce::String("op03FreqAtt");
juce::String UnitySynthesiserAudioProcessor::m_op03FreqDecID = juce::String("op03FreqDec");
juce::String UnitySynthesiserAudioProcessor::m_op03FreqSusID = juce::String("op03FreqSus");
juce::String UnitySynthesiserAudioProcessor::m_op03FreqRelID = juce::String("op03FreqRel");

juce::String UnitySynthesiserAudioProcessor::m_op04FreqID = juce::String("op04Freq");
juce::String UnitySynthesiserAudioProcessor::m_op04DepthID = juce::String("op04Depth");
juce::String UnitySynthesiserAudioProcessor::m_op04AmpAttID = juce::String("op04AmpAtt");
juce::String UnitySynthesiserAudioProcessor::m_op04AmpDecID = juce::String("op04AmpDec");
juce::String UnitySynthesiserAudioProcessor::m_op04AmpSusID = juce::String("op04AmpSus");
juce::String UnitySynthesiserAudioProcessor::m_op04AmpRelID = juce::String("op04AmpRel");
juce::String UnitySynthesiserAudioProcessor::m_op04FreqAttID = juce::String("op04FreqAtt");
juce::String UnitySynthesiserAudioProcessor::m_op04FreqDecID = juce::String("op04FreqDec");
juce::String UnitySynthesiserAudioProcessor::m_op04FreqSusID = juce::String("op04FreqSus");
juce::String UnitySynthesiserAudioProcessor::m_op04FreqRelID = juce::String("op04FreqRel");

juce::String UnitySynthesiserAudioProcessor::m_filterBypassID = juce::String("filterBypass");
juce::String UnitySynthesiserAudioProcessor::m_filterCutoffID = juce::String("filterCutoff");
juce::String UnitySynthesiserAudioProcessor::m_gainID = juce::String("gain");

//Various members need to be initialised in the initializer list
//Most notably the value tree
UnitySynthesiserAudioProcessor::UnitySynthesiserAudioProcessor()
     :  AudioProcessor (BusesProperties()
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
        m_frequencyRange(0, 20000, 1),
        m_envelopeRange(0, 10, 1),
        m_valueTree(*this, nullptr, "Parameters",
                    {
                     std::make_unique<juce::AudioParameterInt>(m_presetID, "Preset", 0, 3, 0),
                     std::make_unique<juce::AudioParameterBool>(m_playingID, "Playing", 0),
                     std::make_unique<juce::AudioParameterChoice>(m_modeID, "Mode", juce::StringArray{"FM", "Noise"}, 0),
                     std::make_unique<juce::AudioParameterFloat>(m_nAttackID, "Noise Attack", 0.1f, 10.0f, m_noiseEnvelopeA),
                     std::make_unique<juce::AudioParameterFloat>(m_nDecayID, "Noise Decay", 0.1f, 10.0f, m_noiseEnvelopeD),
                     std::make_unique<juce::AudioParameterFloat>(m_nSustainID, "Noise Sustain", 0.1f, 1.0f, m_noiseEnvelopeS),
                     std::make_unique<juce::AudioParameterFloat>(m_nReleaseID, "Noise Release", 0.1f, 10.0f, m_noiseEnvelopeR),
                     std::make_unique<juce::AudioParameterInt>(m_algorithmID, "FM Algorithm", 1, 4, 1),
                     std::make_unique<juce::AudioParameterFloat>(m_op01FreqID, "OP01 Freq", m_frequencyRange, m_operator01Frequency),
                     std::make_unique<juce::AudioParameterFloat>(m_op01AmpAttID, "OP01 Amp A", 0.001f, 10.0f, m_operator01AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op01AmpDecID, "OP01 Amp D", 0.001f, 10.0f, m_operator01AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op01AmpSusID, "OP01 Amp S", 0.001f, 1.0f, m_operator01AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op01AmpRelID, "OP01 Amp R", 0.001f, 10.0f, m_operator01AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op01FreqAttID, "OP01 Freq A", 0.001f, 10.0f, m_operator01FreqAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op01FreqDecID, "OP01 Freq D", 0.001f, 10.0f, m_operator01FreqDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op01FreqSusID, "OP01 Freq S", 0.001f, 1.0f, m_operator01FreqSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op01FreqRelID, "OP01 Freq R", 0.001f, 10.0f, m_operator01FreqRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op02FreqID, "OP02 Freq", m_frequencyRange, m_operator02Frequency),
                     std::make_unique<juce::AudioParameterFloat>(m_op02DepthID, "OP02 Depth", 0.0f, 20000.0f, m_operator02Depth),
                     std::make_unique<juce::AudioParameterFloat>(m_op02AmpAttID, "OP02 Amp A", m_envelopeRange, m_operator02AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op02AmpDecID, "OP02 Amp D", m_envelopeRange, m_operator02AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op02AmpSusID, "OP02 Amp S", 0.001f, 1.0f, m_operator02AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op02AmpRelID, "OP02 Amp R", m_envelopeRange, m_operator02AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op02FreqAttID, "OP02 Freq A", m_envelopeRange, m_operator02FreqAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op02FreqDecID, "OP02 Freq D", m_envelopeRange, m_operator02FreqDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op02FreqSusID, "OP02 Freq S", 0.001f, 1.0f, m_operator02FreqSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op02FreqRelID, "OP02 Freq R", m_envelopeRange, m_operator02FreqRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op03FreqID, "OP03 Freq", m_frequencyRange, m_operator03Frequency),
                     std::make_unique<juce::AudioParameterFloat>(m_op03DepthID, "OP03 Depth", 0.0f, 20000.0f, m_operator03Depth),
                     std::make_unique<juce::AudioParameterFloat>(m_op03AmpAttID, "OP03 Amp A",m_envelopeRange, m_operator03AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op03AmpDecID, "OP03 Amp D", m_envelopeRange, m_operator03AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op03AmpSusID, "OP03 Amp S", 0.001f, 1.0f, m_operator03AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op03AmpRelID, "OP03 Amp R", m_envelopeRange, m_operator03AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op03FreqAttID, "OP03 Freq A", m_envelopeRange, m_operator03FreqAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op03FreqDecID, "OP03 Freq D", m_envelopeRange, m_operator03FreqDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op03FreqSusID, "OP03 Freq S", 0.001f, 1.0f, m_operator03FreqSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op03FreqRelID, "OP03 Freq R", m_envelopeRange, m_operator03FreqRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op04FreqID, "OP04 Freq", m_frequencyRange, m_operator04Frequency),
                     std::make_unique<juce::AudioParameterFloat>(m_op04DepthID, "OP04 Depth", 0.0f, 20000.0f, m_operator04Depth),
                     std::make_unique<juce::AudioParameterFloat>(m_op04AmpAttID, "OP04 Amp A", m_envelopeRange, m_operator04AmpAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op04AmpDecID, "OP04 Amp D", m_envelopeRange, m_operator04AmpDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op04AmpSusID, "OP04 Amp S", 0.001f, 1.0f, m_operator04AmpSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op04AmpRelID, "OP04 Amp R", m_envelopeRange, m_operator04AmpRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_op04FreqAttID, "OP04 Freq A", m_envelopeRange, m_operator04FreqAttack),
                     std::make_unique<juce::AudioParameterFloat>(m_op04FreqDecID, "OP04 Freq D", m_envelopeRange, m_operator04FreqDecay),
                     std::make_unique<juce::AudioParameterFloat>(m_op04FreqSusID, "OP04 Freq S", 0.001f, 1.0f, m_operator04FreqSustain),
                     std::make_unique<juce::AudioParameterFloat>(m_op04FreqRelID, "OP04 Freq R", m_envelopeRange, m_operator04FreqRelease),
                     std::make_unique<juce::AudioParameterFloat>(m_gainID, "Gain", 0.0f, 1.0f, 0.5f),
                     std::make_unique<juce::AudioParameterBool>(m_filterBypassID, "Filter Bypass", 1),
                     std::make_unique<juce::AudioParameterFloat>(m_filterCutoffID, "Cutoff", 50.0f, 20000.0f, 20000.0f)
        }),
        m_synthesiser(4)
{
//    Here is the actual constructor the parameters are added to the listener
//    so that their the parametersChanged function is called when
//    their value is changed. Also default paramters are set
    m_valueTree.addParameterListener(m_presetID, this);
    m_valueTree.addParameterListener(m_playingID, this);
    m_valueTree.addParameterListener(m_nAttackID, this);
    m_valueTree.addParameterListener(m_nDecayID, this);
    m_valueTree.addParameterListener(m_nSustainID, this);
    m_valueTree.addParameterListener(m_nReleaseID, this);
    
    m_valueTree.addParameterListener(m_modeID, this);
    
    m_valueTree.addParameterListener(m_algorithmID, this);
    
    m_valueTree.addParameterListener(m_op01FreqID, this);
    m_valueTree.addParameterListener(m_op02FreqID, this);
    m_valueTree.addParameterListener(m_op03FreqID, this);
    m_valueTree.addParameterListener(m_op04FreqID, this);
    
    m_valueTree.addParameterListener(m_op02DepthID, this);
    m_valueTree.addParameterListener(m_op03DepthID, this);
    m_valueTree.addParameterListener(m_op04DepthID, this);
    
    m_valueTree.addParameterListener(m_op01AmpAttID, this);
    m_valueTree.addParameterListener(m_op01AmpDecID, this);
    m_valueTree.addParameterListener(m_op01AmpSusID, this);
    m_valueTree.addParameterListener(m_op01AmpRelID, this);
    m_valueTree.addParameterListener(m_op01FreqAttID, this);
    m_valueTree.addParameterListener(m_op01FreqDecID, this);
    m_valueTree.addParameterListener(m_op01FreqSusID, this);
    m_valueTree.addParameterListener(m_op01FreqRelID, this);
    
    m_valueTree.addParameterListener(m_op02AmpAttID, this);
    m_valueTree.addParameterListener(m_op02AmpDecID, this);
    m_valueTree.addParameterListener(m_op02AmpSusID, this);
    m_valueTree.addParameterListener(m_op02AmpRelID, this);
    m_valueTree.addParameterListener(m_op02FreqAttID, this);
    m_valueTree.addParameterListener(m_op02FreqDecID, this);
    m_valueTree.addParameterListener(m_op02FreqSusID, this);
    m_valueTree.addParameterListener(m_op02FreqRelID, this);
    
    m_valueTree.addParameterListener(m_op03AmpAttID, this);
    m_valueTree.addParameterListener(m_op03AmpDecID, this);
    m_valueTree.addParameterListener(m_op03AmpSusID, this);
    m_valueTree.addParameterListener(m_op03AmpRelID, this);
    m_valueTree.addParameterListener(m_op03FreqAttID, this);
    m_valueTree.addParameterListener(m_op03FreqDecID, this);
    m_valueTree.addParameterListener(m_op03FreqSusID, this);
    m_valueTree.addParameterListener(m_op03FreqRelID, this);
    
    m_valueTree.addParameterListener(m_op04AmpAttID, this);
    m_valueTree.addParameterListener(m_op04AmpDecID, this);
    m_valueTree.addParameterListener(m_op04AmpSusID, this);
    m_valueTree.addParameterListener(m_op04AmpRelID, this);
    m_valueTree.addParameterListener(m_op04FreqAttID, this);
    m_valueTree.addParameterListener(m_op04FreqDecID, this);
    m_valueTree.addParameterListener(m_op04FreqSusID, this);
    m_valueTree.addParameterListener(m_op04FreqRelID, this);
    
    m_valueTree.addParameterListener(m_gainID, this);
    m_valueTree.addParameterListener(m_filterBypassID, this);
    m_valueTree.addParameterListener(m_filterCutoffID, this);
    
    m_synthesiser.getOperator(0)->setFrequency(m_operator01Frequency);
    m_synthesiser.getOperator(1)->setFrequency(m_operator02Frequency);
    m_synthesiser.getOperator(2)->setFrequency(m_operator03Frequency);
    m_synthesiser.getOperator(3)->setFrequency(m_operator04Frequency);
    
    m_synthesiser.getOperator(1)->setModDepth(m_operator02Depth);
    m_synthesiser.getOperator(2)->setModDepth(m_operator03Depth);
    m_synthesiser.getOperator(3)->setModDepth(m_operator04Depth);
    
    m_synthesiser.getOperator(0)->setAmpAttack(m_operator01AmpAttack);
    m_synthesiser.getOperator(0)->setAmpDecay(m_operator01AmpDecay);
    m_synthesiser.getOperator(0)->setAmpSustain(m_operator01AmpSustain);
    m_synthesiser.getOperator(0)->setAmpRelease(m_operator01AmpRelease);
    m_synthesiser.getOperator(0)->setFreqAttack(m_operator01FreqAttack);
    m_synthesiser.getOperator(0)->setFreqDecay(m_operator01FreqDecay);
    m_synthesiser.getOperator(0)->setFreqSustain(m_operator01FreqSustain);
    m_synthesiser.getOperator(0)->setFreqRelease(m_operator01FreqRelease);
    
    m_synthesiser.getOperator(1)->setAmpAttack(m_operator02AmpAttack);
    m_synthesiser.getOperator(1)->setAmpDecay(m_operator02AmpDecay);
    m_synthesiser.getOperator(1)->setAmpSustain(m_operator02AmpSustain);
    m_synthesiser.getOperator(1)->setAmpRelease(m_operator02AmpRelease);
    m_synthesiser.getOperator(1)->setFreqAttack(m_operator02FreqAttack);
    m_synthesiser.getOperator(1)->setFreqDecay(m_operator02FreqDecay);
    m_synthesiser.getOperator(1)->setFreqSustain(m_operator02FreqSustain);
    m_synthesiser.getOperator(1)->setFreqRelease(m_operator02FreqRelease);

    m_synthesiser.getOperator(2)->setAmpAttack(m_operator03AmpAttack);
    m_synthesiser.getOperator(2)->setAmpDecay(m_operator03AmpDecay);
    m_synthesiser.getOperator(2)->setAmpSustain(m_operator03AmpSustain);
    m_synthesiser.getOperator(2)->setAmpRelease(m_operator03AmpRelease);
    m_synthesiser.getOperator(2)->setFreqAttack(m_operator03FreqAttack);
    m_synthesiser.getOperator(2)->setFreqDecay(m_operator03FreqDecay);
    m_synthesiser.getOperator(2)->setFreqSustain(m_operator03FreqSustain);
    m_synthesiser.getOperator(2)->setFreqRelease(m_operator03FreqRelease);
    
    
    m_synthesiser.getOperator(3)->setAmpAttack(m_operator04AmpAttack);
    m_synthesiser.getOperator(3)->setAmpDecay(m_operator04AmpDecay);
    m_synthesiser.getOperator(3)->setAmpSustain(m_operator04AmpSustain);
    m_synthesiser.getOperator(3)->setAmpRelease(m_operator04AmpRelease);
    m_synthesiser.getOperator(3)->setFreqAttack(m_operator03FreqAttack);
    m_synthesiser.getOperator(3)->setFreqDecay(m_operator03FreqDecay);
    m_synthesiser.getOperator(3)->setFreqSustain(m_operator03FreqSustain);
    m_synthesiser.getOperator(3)->setFreqRelease(m_operator03FreqRelease);
    
    
    m_filter.setEnabled(false);
    m_filter.setMode(juce::dsp::LadderFilterMode::LPF12);
    m_filter.setCutoffFrequencyHz(20000.0f);
    m_filter.setResonance(0);
    m_filter.setDrive(1);
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
    return 1;
}

int UnitySynthesiserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UnitySynthesiserAudioProcessor::setCurrentProgram (int index)
{
//    Its worth noting that even though some of these functions
//    in this plugin do not have any logic in them,
//    they still need to be implemented as they
//    are pure virtual functions.
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
//    This function gives us the sample rate currently in use and we can use that
//    to prepare the members that need this information
    m_synthesiser.prepareToPlay(sampleRate);
    
    m_spec.maximumBlockSize = samplesPerBlock;
    m_spec.sampleRate = sampleRate;
    m_spec.numChannels = getTotalNumOutputChannels();
    
    m_gain.prepare(m_spec);
    m_filter.prepare(m_spec);
    m_noiseEnvelope.setSampleRate(sampleRate);
    updateNoiseEnvelopeParameters();
    
    m_gain.setGainLinear(0.5f);
     
}

void UnitySynthesiserAudioProcessor::releaseResources()
{
//    There is no memory or resources that needs to be released
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UnitySynthesiserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
//    The comments in this function are generated by Juce.
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
//    In this function the audio buffer is processed either by filling it with noise
//    or by processing the block in the synthesiser
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
//    This AudioBlock class is actually just a wrapper around the AudioBuffer
//    However, we need to create one for juce dsp classes
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
//    It is important to keep the logic in the correct order
//    For example, here the filter and gain processing need to happen
//    after the synthesiser/noise generator is done with it
    m_filter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    m_gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

//==============================================================================
bool UnitySynthesiserAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* UnitySynthesiserAudioProcessor::createEditor()
{
//     Since we cannot expose parameters in Unity if we provide a custom editor,
//     we need to use the GenericAudioProcessorEditor
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void UnitySynthesiserAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void UnitySynthesiserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UnitySynthesiserAudioProcessor();
}

void UnitySynthesiserAudioProcessor::updateNoiseEnvelopeParameters()
{
    m_noiseEnvelope.setParameters(juce::ADSR::Parameters(m_noiseEnvelopeA, m_noiseEnvelopeD, m_noiseEnvelopeS, m_noiseEnvelopeR));
}

void UnitySynthesiserAudioProcessor::changePreset(float preset)
{
//    This function changes the preset for the plugin
//    Currently four different presets can be selected
    if (preset == 0)
    {
        //Preset 01 A scale
        
        m_valueTree.getParameterAsValue(m_op01FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(57);
        m_valueTree.getParameterAsValue(m_op02FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(69);
        m_valueTree.getParameterAsValue(m_op03FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(81);
        m_valueTree.getParameterAsValue(m_op04FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(93);
        
        DBG("Preset01: "+juce::String(m_valueTree.getRawParameterValue(m_op01FreqID)->load()));
        
    }
    else if (preset == 1)
    {
        //Preset 02 C scale
        
        m_valueTree.getParameterAsValue(m_op01FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(48);
        m_valueTree.getParameterAsValue(m_op02FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(60);
        m_valueTree.getParameterAsValue(m_op03FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(72);
        m_valueTree.getParameterAsValue(m_op04FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(84);
        
        DBG("Preset02: "+juce::String(m_valueTree.getRawParameterValue(m_op01FreqID)->load()));
    }
    else if (preset == 2)
    {
        //Preset 03 D scale
        
        m_valueTree.getParameterAsValue(m_op01FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(50);
        m_valueTree.getParameterAsValue(m_op02FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(62);
        m_valueTree.getParameterAsValue(m_op03FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(74);
        m_valueTree.getParameterAsValue(m_op04FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(86);
        
        DBG("Preset03: "+juce::String(m_valueTree.getRawParameterValue(m_op01FreqID)->load()));

    }
    else if (preset == 3)
    {
        //Preset 04 E scale
        
        m_valueTree.getParameterAsValue(m_op01FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(52);
        m_valueTree.getParameterAsValue(m_op02FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(64);
        m_valueTree.getParameterAsValue(m_op03FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(76);
        m_valueTree.getParameterAsValue(m_op04FreqID) = (float) juce::MidiMessage::getMidiNoteInHertz(88);
        
    }

}

void UnitySynthesiserAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
//    This function is used to update the parameters of the plugin as they are
//    interacted with.
    if (parameterID == m_presetID)
    {
        changePreset(newValue);
    }
    
    else if (parameterID == m_playingID)
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
    
    
    else if (parameterID == m_nAttackID)
    {
        m_noiseEnvelopeA = newValue;
        updateNoiseEnvelopeParameters();
        
    }
    else if (parameterID == m_nDecayID)
    {
        m_noiseEnvelopeD = newValue;
        updateNoiseEnvelopeParameters();
    }
    else if (parameterID == m_nSustainID)
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
    else if (parameterID == m_nReleaseID)
    {
        m_noiseEnvelopeR = newValue;
        updateNoiseEnvelopeParameters();
    }
    
    else if (parameterID == m_modeID)
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
    else if (parameterID == m_algorithmID)
    {
        m_synthesiser.changeAlgorithm((int) newValue);
    }
    
    
    else if (parameterID == m_op01FreqID)
    {
        m_synthesiser.getOperator(0)->setFrequency(newValue);
    }
    else if (parameterID == m_op02FreqID)
    {
        m_synthesiser.getOperator(1)->setFrequency(newValue);
    }
    else if (parameterID == m_op02DepthID)
    {
        m_synthesiser.getOperator(1)->setModDepth(newValue);
    }
    else if (parameterID == m_op03FreqID)
    {
        m_synthesiser.getOperator(2)->setFrequency(newValue);
    }
    else if (parameterID == m_op03DepthID)
    {
        m_synthesiser.getOperator(2)->setModDepth(newValue);
    }
    else if (parameterID == m_op04FreqID)
    {
        m_synthesiser.getOperator(3)->setFrequency(newValue);
    }
    else if (parameterID == m_op04DepthID)
    {
        m_synthesiser.getOperator(3)->setModDepth(newValue);
    }
    else if (parameterID == m_op01AmpAttID)
    {
        m_synthesiser.getOperator(0)->setAmpAttack(newValue);
    }
    else if (parameterID == m_op01AmpDecID)
    {
        m_synthesiser.getOperator(0)->setAmpDecay(newValue);
    }
    else if (parameterID == m_op01AmpSusID)
    {
        m_synthesiser.getOperator(0)->setAmpSustain(newValue);
    }
    else if (parameterID == m_op01AmpRelID)
    {
        m_synthesiser.getOperator(0)->setAmpRelease(newValue);
    }
    else if (parameterID == m_op02AmpAttID)
    {
        m_synthesiser.getOperator(1)->setAmpAttack(newValue);
    }
    else if (parameterID == m_op02AmpDecID)
    {
        m_synthesiser.getOperator(1)->setAmpDecay(newValue);
    }
    else if (parameterID == m_op02AmpSusID)
    {
        m_synthesiser.getOperator(1)->setAmpSustain(newValue);
    }
    else if (parameterID == m_op02AmpRelID)
    {
        m_synthesiser.getOperator(1)->setAmpRelease(newValue);
    }
    else if (parameterID == m_op03AmpAttID)
    {
        m_synthesiser.getOperator(2)->setAmpAttack(newValue);
    }
    else if (parameterID == m_op03AmpDecID)
    {
        m_synthesiser.getOperator(2)->setAmpDecay(newValue);
    }
    else if (parameterID == m_op03AmpSusID)
    {
        m_synthesiser.getOperator(2)->setAmpSustain(newValue);
    }
    else if (parameterID == m_op03AmpRelID)
    {
        m_synthesiser.getOperator(2)->setAmpRelease(newValue);
    }
    else if (parameterID == m_op04AmpAttID)
    {
        m_synthesiser.getOperator(3)->setAmpAttack(newValue);
    }
    else if (parameterID == m_op04AmpDecID)
    {
        m_synthesiser.getOperator(3)->setAmpDecay(newValue);
    }
    else if (parameterID == m_op04AmpSusID)
    {
        m_synthesiser.getOperator(3)->setAmpSustain(newValue);
    }
    else if (parameterID == m_op04AmpRelID)
    {
        m_synthesiser.getOperator(3)->setAmpRelease(newValue);
    }
    else if (parameterID == m_op01FreqAttID)
    {
        m_synthesiser.getOperator(0)->setFreqAttack(newValue);
    }
    else if (parameterID == m_op01FreqDecID)
    {
        m_synthesiser.getOperator(0)->setFreqDecay(newValue);
    }
    else if (parameterID == m_op01FreqSusID)
    {
        m_synthesiser.getOperator(0)->setFreqSustain(newValue);
    }
    else if (parameterID == m_op01FreqRelID)
    {
        m_synthesiser.getOperator(0)->setFreqRelease(newValue);
    }
    else if (parameterID == m_op02FreqAttID)
    {
        m_synthesiser.getOperator(1)->setFreqAttack(newValue);
    }
    else if (parameterID == m_op02FreqDecID)
    {
        m_synthesiser.getOperator(1)->setFreqDecay(newValue);
    }
    else if (parameterID == m_op02FreqSusID)
    {
        m_synthesiser.getOperator(1)->setFreqSustain(newValue);
    }
    else if (parameterID == m_op02FreqRelID)
    {
        m_synthesiser.getOperator(1)->setFreqRelease(newValue);
    }
    else if (parameterID == m_op03FreqAttID)
    {
        m_synthesiser.getOperator(2)->setFreqAttack(newValue);
    }
    else if (parameterID == m_op03FreqDecID)
    {
        m_synthesiser.getOperator(2)->setFreqDecay(newValue);
    }
    else if (parameterID == m_op03FreqSusID)
    {
        m_synthesiser.getOperator(2)->setFreqSustain(newValue);
    }
    else if (parameterID == m_op03FreqRelID)
    {
        m_synthesiser.getOperator(2)->setFreqRelease(newValue);
    }
    else if (parameterID == m_op04FreqAttID)
    {
        m_synthesiser.getOperator(3)->setFreqAttack(newValue);
    }
    else if (parameterID == m_op04FreqDecID)
    {
        m_synthesiser.getOperator(3)->setFreqDecay(newValue);
    }
    else if (parameterID == m_op04FreqSusID)
    {
        m_synthesiser.getOperator(3)->setFreqSustain(newValue);
    }
    else if (parameterID == m_op04FreqRelID)
    {
        m_synthesiser.getOperator(3)->setFreqRelease(newValue);
    }
    else if (parameterID == m_gainID)
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
    else if (parameterID == m_filterBypassID)
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
    else if (parameterID == m_filterCutoffID)
    {
        m_filter.setCutoffFrequencyHz(newValue);
    }
    DBG("Parameter " + parameterID + " is now: " + juce::String(newValue));
}

