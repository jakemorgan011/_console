/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <regex>



class interpreter {
public:
    
    void parseCommand(const std::string& command, AU_testAudioProcessor& p){
        std::regex dist_amnt_pattern(R"(^set_dist\s+(-?\d+)$)");
        std::regex dry_wet_pattern(R"(^set_drywet\s+(-?\d+)$)");
        std::regex type_pattern(R"(^set_type\s+(-?\d+)$)");
        std::regex hi("^hi$");
        std::regex list("^show_list$");
        std::regex lfo_frequency_pattern(R"(^set_lfo_freq\s+(\d+(?:\.\d+)?)$)");
        std::regex lfo_type_pattern(R"(^set_lfo_type\s+(-?\d+)$)");
        std::regex lfo_phase_pattern(R"(^set_lfo_phase\s+(-?\d+)$)");
        
        std::smatch match;
        
        if (std::regex_match(command, match, dist_amnt_pattern)){
            int dist_amnt = std::stoi(match[1].str());
            p.input_string = command;
            p.temporary_dist = dist_amnt;
            
            // assigning here because ValueTreeState is not constructed on launch.
            //TODO: MAKE THIS FUCNTION WITH META PARAMETERS SO THAT YOU CAN USE IT IN LOGIC
            /*
            p.distParam = p.ValueTreeState->getParameter("dist");
            p.distParam->beginChangeGesture();
            p.distParam->setValueNotifyingHost(dist_amnt/100.f);
            p.distParam->endChangeGesture();
            */
            p.output_string = "distus amountus setus " + std::to_string(dist_amnt) + "/100";
        }else if (std::regex_match(command, match, dry_wet_pattern)){
            int dry_wet_amnt = std::stoi(match[1].str());

            p.temporary_dry_wet = dry_wet_amnt;
            p.input_string = command;
            
            // assigning here because ValueTreeState is not constructed on launch.
            /*
            p.dryWetParam = p.ValueTreeState->getParameter("dry_wet");
            p.dryWetParam->beginChangeGesture();
            p.dryWetParam->setValueNotifyingHost(dry_wet_amnt/100.f);
            p.dryWetParam->endChangeGesture();
            */
            
            p.output_string = "dryus wetus setus" + std::to_string(dry_wet_amnt) + "/100";
        }else if (std::regex_match(command, match, type_pattern)){
            int type = std::stoi(match[1].str());
            
            //TODO: type is for some reason not working. figure it out.
            p.temporary_type = type;
            p.input_string = command;
            DBG(type);
            
            // assigning here because ValueTreeState is not constructed on launch.
            /*
            p.typeParam = p.ValueTreeState->getParameter("type");
            p.typeParam->beginChangeGesture();
            p.typeParam->setValueNotifyingHost(type);
            p.typeParam->endChangeGesture();
            DBG(*p.type);
            */
            
            p.output_string = "typus setus " + std::to_string(type);
        }else if (std::regex_match(command, match, hi)){
            p.output_string = "haiiiiii :p";
        }else if(std::regex_match(command, match, list)){
            p.showList = true;
            p.output_string = "commands... show yourself";
        }else if(std::regex_match(command, match, lfo_frequency_pattern)){
            
        }else if(std::regex_match(command, match, lfo_type_pattern)){
            
        }else if(std::regex_match(command, match, lfo_phase_pattern)){
            
        }
        else{
            p.output_string = "i dont know what you are talking about";
        }
    }
};
//ALMOST WORKING
// ===============================================
//
//              THE LISTENER CLASS
//
// ===============================================

class customListener : public juce::TextEditor::Listener {
public:
    
    void setProcessor(AU_testAudioProcessor& p){
        processor = &p;
    }
    
    void textEditorReturnKeyPressed (juce::TextEditor& t) override {
        //std::cout << "enter pressed; posting from listener class" << std::endl;
        editor_text.reset(new termite::string(t.getText().toUTF8()));
        std::cout << editor_text->returnCharacters() << std::endl;
        interpreter.parseCommand(editor_text->returnCharacters(), *processor);
    }
    
    AU_testAudioProcessor* processor;
private:
    interpreter interpreter;
    std::unique_ptr<termite::string> editor_text;
};

class customLabel : public juce::Label {
    
};

//==============================================================================
/**
*/
class AU_testAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AU_testAudioProcessorEditor (AU_testAudioProcessor&);
    ~AU_testAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::TextEditor textEditor;
    customListener customListener;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AU_testAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AU_testAudioProcessorEditor)
};


