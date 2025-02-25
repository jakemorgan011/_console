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
class _consoleAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    _consoleAudioProcessorEditor (_consoleAudioProcessor&);
    ~_consoleAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //
    /*
    void updateDJ(int sampleRate, long long int currentSample){
        int update_frame = sampleRate/dj_framerate;
        if(currentSample%update_frame == 0){
            switch(dj_case){
                case 0:
                    dj = juce::ImageCache::getFromMemory(BinaryData::djone_png, BinaryData::djone_pngSize);
                    repaint();
                    break;
                case 1:
                    dj = juce::ImageCache::getFromMemory(BinaryData::djtwo_png, BinaryData::djtwo_pngSize);
                    repaint();
                    break;
            }
            dj_case--;
            dj_case = std::abs(dj_case);
        }
    };
    */
    
    void updateDJ(){
        switch(dj_case){
            case 0:
                dj = juce::ImageCache::getFromMemory(BinaryData::djone_png, BinaryData::djone_pngSize);
                repaint();
                break;
            case 1:
                dj = juce::ImageCache::getFromMemory(BinaryData::djtwo_png, BinaryData::djtwo_pngSize);
                repaint();
                break;
            }
        dj_case--;
        dj_case = std::abs(dj_case);
    };
    
    void updatePuffle(){
        switch(puffle_case){
            case 0:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_1_png, BinaryData::pf_1_pngSize);
                repaint();
                break;
            case 1:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_2_png, BinaryData::pf_2_pngSize);
                repaint();
                break;
            case 2:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_3_png, BinaryData::pf_3_pngSize);
                repaint();
                break;
            case 3:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_4_png, BinaryData::pf_4_pngSize);
                repaint();
                break;
            case 4:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_5_png, BinaryData::pf_5_pngSize);
                repaint();
                break;
            case 5:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_6_png, BinaryData::pf_6_pngSize);
                repaint();
                break;
            case 6:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_7_png, BinaryData::pf_7_pngSize);
                repaint();
                break;
            case 7:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_8_png, BinaryData::pf_8_pngSize);
                repaint();
                break;
            case 8:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_9_png, BinaryData::pf_9_pngSize);
                repaint();
                break;
            case 9:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_10_png, BinaryData::pf_10_pngSize);
                repaint();
                break;
            case 10:
                puffle = juce::ImageCache::getFromMemory(BinaryData::pf_11_png, BinaryData::pf_11_pngSize);
                repaint();
                break;
        }
        puffle_case++;
        if(puffle_case > 10){
            puffle_case = 0;
        }
    }
    
    void timerCallback() override {
        if(frame_counter == 11 || frame_counter == 0){
            updateDJ();
        }
        if(frame_counter == 0 || frame_counter%2 == 0){
            updatePuffle();
        }
        frame_counter++;
        if(frame_counter > 21){
            frame_counter = 0;
        }
    };
    
    
    //
    
    customListener customListener;
    std::string command;

private:
    
    int frame_counter = 0;
    
    std::unique_ptr<termite::string> log;
    std::string sLog;
    
    std::string list = "command list: \nset_dist <num> \nset_type <num> \nset_drywet <num> \nshow_list \n \nthe wizard can't handle extraneous spaces btw... be nice to him";
    
    std::string distLog;
    std::string typeLog;
    std::string dryWetLog;
    
    juce::Label consoleLabel;
    juce::Image walter;
    juce::Image wiseOldMan;
    juce::Image dj;
    juce::Image frame;
    juce::Image puffle;
    
    // framerate equation: fr = spritecount*updates_per_second;
    int dj_framerate = 2;
    int dj_case = 0;
    
    int puffle_case = 0;
    
    juce::TextEditor valueList;
    juce::TextEditor textEditor;
    juce::TextEditor::Listener textListener;
    juce::TextEditor consoleLog;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _consoleAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_consoleAudioProcessorEditor)
};
