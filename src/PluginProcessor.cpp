/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AU_testAudioProcessor::AU_testAudioProcessor()
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
}

AU_testAudioProcessor::~AU_testAudioProcessor()
{
}

//==============================================================================
const juce::String AU_testAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AU_testAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AU_testAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AU_testAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AU_testAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AU_testAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AU_testAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AU_testAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AU_testAudioProcessor::getProgramName (int index)
{
    return {};
}

void AU_testAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AU_testAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void AU_testAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AU_testAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AU_testAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::AudioPlayHead* playHead = getPlayHead();
    if(playHead != nullptr){
        juce::AudioPlayHead::CurrentPositionInfo currentPosition;
        time_in_samples = currentPosition.timeInSamples;
    }
    
    float* in_left = buffer.getWritePointer(0);
    float* in_right = buffer.getWritePointer(1);
    
    int num_samps = buffer.getNumSamples();
    
    juce::AudioBuffer<float> dryBuff;
    juce::AudioBuffer<float> wetBuff;
    
    dryBuff.setSize(2, 2048);
    wetBuff.setSize(2, 2048);
    dryBuff.clear();
    wetBuff.clear();
    
    float* dry_left = dryBuff.getWritePointer(0);
    float* dry_right = dryBuff.getWritePointer(1);
    
    float* wet_left = wetBuff.getWritePointer(0);
    float* wet_right = wetBuff.getWritePointer(1);
    
    
    for (int i = 0; i < num_samps; i++){
        switch(temporary_type){
            case 0:
                // hard clip
                if(in_left[i]*temporary_dist/50.f > 1){
                    wet_left[i] = 1;
                }
                else if(in_left[i]*temporary_dist/50.f < -1){
                    wet_left[i] = -1;
                }
                
                else if(in_right[i]*temporary_dist/50.f > 1){
                    wet_right[i] = 1;
                }
                else if(in_right[i]*temporary_dist/50.f < -1){
                    wet_right[i] = -1;
                }
                break;
                
            case 1:
                // soft clip
                wet_left[i] = tanh((in_left[i]*(temporary_dist/50.f)));
                wet_right[i] = tanh((in_right[i]*(temporary_dist/50.f)));
                break;
                
                // misc algorithms
            case 2:
                wet_left[i] = tanh(in_left[i]*tan(in_left[i]*(temporary_dist/50.f))*temporary_dist/50.f);
                wet_right[i] = tanh(in_right[i]*tan(in_right[i]*(temporary_dist/50.f))*temporary_dist/50.f);
                break;
                
                // this does nothing lol
            case 3:
                wet_left[i] = tanh(in_left[i]*(sin(in_left[i])/tan(in_left[i]*(temporary_dist/50.f))*temporary_dist));
                wet_right[i] = tanh(in_right[i]*(sin(in_right[i])/tan(in_right[i]*(temporary_dist/50.f))*temporary_dist));
                break;
            
        }
        
        dry_left[i] = in_left[i]*(1-(temporary_dry_wet/100.f));
        dry_right[i] = in_right[i]*(1-(temporary_dry_wet/100.f));
        
        wet_left[i] = wet_left[i]*(temporary_dry_wet/100.f);
        wet_right[i] = wet_right[i]*(temporary_dry_wet/100.f);
        
        in_left[i] = dry_left[i] + wet_left[i];
        in_right[i] = dry_right[i] + wet_right[i];
        
    }
}

//==============================================================================
bool AU_testAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AU_testAudioProcessor::createEditor()
{
    return new AU_testAudioProcessorEditor (*this);
}

//==============================================================================
void AU_testAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AU_testAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AU_testAudioProcessor();
}
