/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_consoleAudioProcessor::_consoleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    _constructValueTreeState();
    dist = ValueTreeState->getRawParameterValue("dist");
    dry_wet = ValueTreeState->getRawParameterValue("dry_wet");
}

_consoleAudioProcessor::~_consoleAudioProcessor()
{
}

//==============================================================================
const juce::String _consoleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _consoleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _consoleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _consoleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _consoleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _consoleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _consoleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _consoleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _consoleAudioProcessor::getProgramName (int index)
{
    return {};
}

void _consoleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _consoleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void _consoleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _consoleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _consoleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    float* in_left = buffer.getWritePointer(0);
    float* in_right = buffer.getWritePointer(1);
    
    int num_samps = buffer.getNumSamples();
    
    for (int i = 0; i < num_samps; i++){
        in_left[i] = tanh((in_left[i])*((temporary_dist/50.f)));
        in_right[i] = tanh((in_left[i])*(temporary_dist/50.f));
    }
    
}

//==============================================================================
bool _consoleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _consoleAudioProcessor::createEditor()
{
    return new _consoleAudioProcessorEditor (*this);
}

//==============================================================================
void _consoleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _consoleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _consoleAudioProcessor();
}

void _consoleAudioProcessor::_constructValueTreeState(){
    ValueTreeState.reset(new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("_console"),{
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dist", 1), "dist", juce::NormalisableRange<float>(0.f,1.f,0.01f), 0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dry_wet", 1), "dry_wet", juce::NormalisableRange<float>(0.f,1.f,0.01f), 0.5f)
        
    }));
}
