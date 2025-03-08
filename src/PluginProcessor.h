/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class AU_testAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AU_testAudioProcessor();
    ~AU_testAudioProcessor() override;

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

    float temporary_dist;
    float temporary_dry_wet;
    int temporary_type;
    std::string input_string;
    std::string output_string;
    bool showList = false;
    
    long long int time_in_samples;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AU_testAudioProcessor)
};

namespace termite{
class string{
public:
    // default constructor.
    string() : data(nullptr), length(0) {}
    //c style constructor.
    string(const char* str){
        length = 0;
        while (str[length] != '\0'){
            length++;
        }
        
        data = new char[length +1];
        for(size_t i = 0; i < length; i++){
            data[i] = str[i];
        }
        data[length] = '\0';
    }
    // deletes the data
    ~string(){
        delete[] data;
    }
    
    // returns size
    size_t size() const {
        return length;
    }
    
    // overloads the operator= for printing.
    friend std::ostream& operator<<(std::ostream& os, const string& str){
        if(str.data){
            os << str.data;
        }
        return os;
    }
    
    // overloads the operator= for assignment
    // theres probably a way to overload the
    string& operator=(const string& other){
        if(this == &other) return *this; //self assignment check
        
        delete[] data;
        
        length= other.length;
        data = new char[length + 1];
        for(size_t i = 0; i < length; i++){
            data[i] = other.data[i];
        }
        data[length] = '\0';
        return *this;
    }
    
    
    char* returnCharacters(){
        return data;
    }
private:
    
    char* data;
    size_t length;
    
};
}
