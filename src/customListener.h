/*
  ==============================================================================

    customListener.h
    Created: 11 Jan 2025 2:31:42pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "termite_data_types.h"
#include <regex>

//TODO: allow parser to interact with the text in the listener and interpret the text
/// done! sort of....

// parsing =================
struct parse_node {
    std::string value;
    std::vector<std::shared_ptr<parse_node>> children;
    
    // this basically says that whenever you want to use this data type you have to declare it this way
    // the : declares that the value is set via declaration.
    explicit parse_node(const std::string& val) : value(val){}
};

// might be nice to deconstruct this example class and then use the termite string instead of the std class
class lexer{
public:
    explicit lexer(const std::string& input) : input(input), pos(0) {
        tokenize();
    }
    std::string getNextToken(){
        if (current_token_index < tokens.size()){
            return tokens[current_token_index++];
        }
        return "";
    }
    
    std::string peek() const {
        if (current_token_index < tokens.size()) {
            return tokens[current_token_index];
        }
        return "";
    }
private:
    std::string input;
    size_t pos;
    std::vector<std::string> tokens;
    size_t current_token_index = 0;
    
    // essentially this adds to the vector of strings when a space is detected and creates a new "token"  or word or character etc.
    // push_back is very similar to .push() in js
    void tokenize(){
        // why unsigned type here ???
        // works but just wondering why i should do this.
        /// answer: unsigned because it will only be positive???
        size_t start = 0;
        while (pos < input.size()){
            if(isspace(input[pos])){
                if(input[pos] == '\n'){ /// technically don't need the new line but leave for now.
                    if (start < pos) tokens.push_back(input.substr(start, pos - start));
                    tokens.push_back("\n");
                    start = ++pos;
                }else{
                    if (start < pos) tokens.push_back(input.substr(start, pos - start));
                    start = ++pos;
                }
            }else {
                pos++;
            }
        }
        if(start < pos) tokens.push_back(input.substr(start, pos - start));
    }
};

class parser {
public:
    explicit parser(lexer& lexer) : lexer(lexer) {}
    std::shared_ptr<parse_node> parse(){
        return parse_text();
    }
private:
    lexer& lexer;
    
    std::shared_ptr<parse_node> parse_text(){
        auto textNode = std::make_shared<parse_node>("text");
        textNode->children.push_back(parseSetVal());
        return textNode;
    }
    
    std::shared_ptr<parse_node> parseSetVal(){
        std::regex dist_amnt_pattern(R"(^set_dist\s+(-?\d+)$)");
        std::regex dry_wet_pattern(R"()");
        std::string token = lexer.getNextToken();
        return(std::make_shared<parse_node>("fix later maybe"));
    }
    
};


// this is the super easy way and bad for runtime.
//TODO: change this in the future using the parsing method above.


class interpreter {
public:
    struct commandData {
        size_t data;
        std::unique_ptr<termite::string> string;
    };
    
    commandData commandData;
    
    void parseCommand(const std::string& command, _consoleAudioProcessor& p){
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
            p.distParam = p.ValueTreeState->getParameter("dist");
            p.distParam->beginChangeGesture();
            p.distParam->setValueNotifyingHost(dist_amnt/100.f);
            p.distParam->endChangeGesture();
            
            p.output_string = "distus amountus setus " + std::to_string(dist_amnt) + "/100";
        }else if (std::regex_match(command, match, dry_wet_pattern)){
            int dry_wet_amnt = std::stoi(match[1].str());

            p.temporary_dry_wet = dry_wet_amnt;
            p.input_string = command;
            
            // assigning here because ValueTreeState is not constructed on launch.
            p.dryWetParam = p.ValueTreeState->getParameter("dry_wet");
            p.dryWetParam->beginChangeGesture();
            p.dryWetParam->setValueNotifyingHost(dry_wet_amnt/100.f);
            p.dryWetParam->endChangeGesture();
            
            p.output_string = "dryus wetus setus" + std::to_string(dry_wet_amnt) + "/100";
        }else if (std::regex_match(command, match, type_pattern)){
            int type = std::stoi(match[1].str());
            
            //TODO: type is for some reason not working. figure it out.
            p.temporary_type = type;
            p.input_string = command;
            DBG(type);
            
            // assigning here because ValueTreeState is not constructed on launch.
            p.typeParam = p.ValueTreeState->getParameter("type");
            p.typeParam->beginChangeGesture();
            p.typeParam->setValueNotifyingHost(type);
            p.typeParam->endChangeGesture();
            DBG(*p.type);
            
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
    
    void setProcessor(_consoleAudioProcessor& p){
        processor = &p;
    }
    
    void textEditorReturnKeyPressed (juce::TextEditor& t) override {
        //std::cout << "enter pressed; posting from listener class" << std::endl;
        editor_text.reset(new termite::string(t.getText().toUTF8()));
        std::cout << editor_text->returnCharacters() << std::endl;
        interpreter.parseCommand(editor_text->returnCharacters(), *processor);
    }
    
    _consoleAudioProcessor* processor;
private:
    interpreter interpreter;
    std::unique_ptr<termite::string> editor_text;
};

class customLabel : public juce::Label {
    
};
