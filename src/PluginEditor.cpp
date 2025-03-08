/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AU_testAudioProcessorEditor::AU_testAudioProcessorEditor (AU_testAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    textEditor.addListener(&customListener);
    customListener.setProcessor(audioProcessor);
    textEditor.setFont(juce::Font(14));
    textEditor.setMultiLine(true);
    textEditor.setOpaque(false);
    textEditor.setAlpha(0.5);
    textEditor.setReturnKeyStartsNewLine(false);
    
    textEditor.onReturnKey = [&, this](){
        textEditor.clear();
    };
    
    addAndMakeVisible(textEditor);
    
    
    setSize (200, 200);
}

AU_testAudioProcessorEditor::~AU_testAudioProcessorEditor()
{
}

//==============================================================================
void AU_testAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
}

void AU_testAudioProcessorEditor::resized()
{
    textEditor.setBounds(getLocalBounds());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
