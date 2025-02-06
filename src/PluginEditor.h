/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "customListener.h"

//==============================================================================
/**
*/
class _consoleAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _consoleAudioProcessorEditor (_consoleAudioProcessor&);
    ~_consoleAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    customListener customListener;
    std::string command;

private:
    
    juce::Label consoleLabel;
    juce::Image walter;
    
    juce::TextEditor textEditor;
    juce::TextEditor::Listener textListener;
    juce::TextEditor consoleLog;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _consoleAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_consoleAudioProcessorEditor)
};
