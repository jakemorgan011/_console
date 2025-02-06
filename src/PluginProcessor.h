/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "termite_data_types.h"

//==============================================================================
/**
*/
class _consoleAudioProcessor  : public juce::AudioProcessor
{
public:
    
    struct commandData {
        size_t data;
        std::unique_ptr<termite::string> string;
    };
    //==============================================================================
    _consoleAudioProcessor();
    ~_consoleAudioProcessor() override;

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
    
    juce::AudioProcessorValueTreeState& getVTS(){
        return *ValueTreeState;
    }
    
    float temporary_dist;
    float temporary_dry_wet;
    std::string input_string;
    
    
private:
    
    
    std::atomic<float>* dist = nullptr;
    std::atomic<float>* dry_wet = nullptr;
    
    juce::LinearSmoothedValue<float> smoothedDist;
    juce::LinearSmoothedValue<float> smoothedDryWet;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState> ValueTreeState;
    
    commandData distCommand;
    commandData dryWetCommand;
    
    void _constructValueTreeState();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_consoleAudioProcessor)
};
