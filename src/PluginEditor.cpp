/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_consoleAudioProcessorEditor::_consoleAudioProcessorEditor (_consoleAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //textEditor.addListener(&textListener);
    textEditor.addListener(&customListener);
    customListener.setProcessor(audioProcessor);
    textEditor.setFont(juce::Font(14));
    textEditor.setMultiLine(true);
    textEditor.setOpaque(false);
    textEditor.setReturnKeyStartsNewLine(false);
    textEditor.onReturnKey = [this](){
        textEditor.clear();
        command = textEditor.getText().toUTF8();
        // this is not working.
        consoleLabel.setText(command, juce::NotificationType::dontSendNotification);
    };
    textEditor.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::grey);
    textEditor.setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::white);
    textEditor.setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::white);
    textEditor.setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::black);
    addAndMakeVisible(textEditor);
    
    consoleLog.setFont(juce::Font(14));
    consoleLog.setMultiLine(true);
    consoleLog.setOpaque(false);
    consoleLog.setReadOnly(true);
    consoleLog.setReturnKeyStartsNewLine(false);
    consoleLog.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::grey);
    consoleLog.setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::white);
    consoleLog.setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::white);
    consoleLog.setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::black);
    //addAndMakeVisible(consoleLog);
    consoleLabel.setEnabled(true);
    addAndMakeVisible(consoleLabel);
    
    setSize (300, 300);
}

_consoleAudioProcessorEditor::~_consoleAudioProcessorEditor()
{
}

//==============================================================================
void _consoleAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void _consoleAudioProcessorEditor::resized()
{
    textEditor.setBounds(0, 0, 300, 100);
    //consoleLog.setBounds(0,100,300,200);
    consoleLabel.setBounds(0, 100, 300, 200);
}
