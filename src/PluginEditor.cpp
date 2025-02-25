
//TODO: OKAY GOOD NOTES:
/// maybe just lean into text adventure and keep the whole WOM style
//TODO: ADD LOG OF COMMANDS TO REFERENCE. MAYBE A LIST OF PARAMETERS ON THE SIDE AND THEIR CURRENT STATES.
//TODO: THE LIST OF PARAMETERS IDEA IS REALLY GOOD.
//TODO: IM PRETTY SURE THE COMMANDS CAN DIRECTLY INTERACT WITH THE VALUE TREE
//TODO: BUILD A VERSION THAT IS ALL KEYBINDS. GO CRAZY.

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_consoleAudioProcessorEditor::_consoleAudioProcessorEditor (_consoleAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    walter = juce::ImageCache::getFromMemory(BinaryData::walter_jpeg, BinaryData::walter_jpegSize);
    wiseOldMan = juce::ImageCache::getFromMemory(BinaryData::WOM_standing_jpg, BinaryData::WOM_standing_jpgSize);
    dj = juce::ImageCache::getFromMemory(BinaryData::djtwo_png, BinaryData::djtwo_pngSize);
    frame = juce::ImageCache::getFromMemory(BinaryData::frame_png, BinaryData::frame_pngSize);
    // timer
    startTimer(35);
    
    //textEditor.addListener(&textListener);
    textEditor.addListener(&customListener);
    customListener.setProcessor(audioProcessor);
    textEditor.setFont(juce::Font(14));
    textEditor.setMultiLine(true);
    textEditor.setOpaque(false);
    textEditor.setAlpha(0.5);
    textEditor.setReturnKeyStartsNewLine(false);
    
    //
    textEditor.onFocusLost = [this](){
        wiseOldMan = juce::ImageCache::getFromMemory(BinaryData::WOM_standing_jpg, BinaryData::WOM_standing_jpgSize);
        repaint();
    };
    textEditor.onTextChange = [this](){
        wiseOldMan = juce::ImageCache::getFromMemory(BinaryData::WOM_read_jpg, BinaryData::WOM_read_jpgSize);
        consoleLabel.setText(textEditor.getText(), juce::NotificationType::dontSendNotification);
        repaint();
    };
    textEditor.onReturnKey = [&, this](){
        log.reset(new termite::string(textEditor.getText().toUTF8()));
        textEditor.clear();
        wiseOldMan = juce::ImageCache::getFromMemory(BinaryData::WOM_celebrate_png, BinaryData::WOM_celebrate_pngSize);
        consoleLabel.setText(audioProcessor.output_string, juce::NotificationType::dontSendNotification);
        
        //
        int distCast = static_cast<int>(p.temporary_dist);
        int dryWetCast = static_cast<int>(p.temporary_dry_wet);
        
        distLog = "distortion: " + std::to_string(distCast) + "/100\n";
        dryWetLog = "dry/wet: " + std::to_string(dryWetCast) + "/100\n";
        typeLog = "type: " + std::to_string(p.temporary_type) + "\n";
        valueList.setText(distLog + dryWetLog + typeLog);
        //
        // very weird way of doing this ig
        //TODO: make a cascading history of all commands inserted
        std::string nLog = log->returnCharacters();
        sLog = sLog + "\n";
        sLog = sLog + nLog;
        if(audioProcessor.showList){
            sLog = list;
            audioProcessor.showList = false;
        }
        //consoleLog.setText(audioProcessor.input_string);
        consoleLog.setText(sLog);
        repaint();
    };
    //
    
    
    textEditor.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    textEditor.setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    textEditor.setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::red);
    textEditor.setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::white);
    textEditor.setColour(juce::TextEditor::ColourIds::focusedOutlineColourId, juce::Colours::red);
    textEditor.setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colours::black);
    addAndMakeVisible(textEditor);
    
    consoleLog.setFont(juce::Font(14));
    consoleLog.setMultiLine(true);
    consoleLog.setOpaque(true);
    consoleLog.setReadOnly(true);
    consoleLog.setReturnKeyStartsNewLine(false);
    consoleLog.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    consoleLog.setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    consoleLog.setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::red);
    consoleLog.setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::white);
    consoleLog.setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colours::white);
    // only appears on plugin launch
    consoleLog.setText(list);
    //
    addAndMakeVisible(consoleLog);
    consoleLabel.setEnabled(true);
    consoleLabel.setFont(juce::Font(16));
    consoleLabel.setJustificationType(juce::Justification::topLeft);
    addAndMakeVisible(consoleLabel);
    
    valueList.setFont(juce::Font(14));
    valueList.setMultiLine(true);
    valueList.setOpaque(true);
    valueList.setReadOnly(true);
    valueList.setReturnKeyStartsNewLine(false);
    valueList.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::white);
    valueList.setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::black);
    valueList.setColour(juce::TextEditor::ColourIds::highlightColourId, juce::Colours::red);
    valueList.setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::white);
    valueList.setColour(juce::TextEditor::ColourIds::outlineColourId, juce::Colours::white);
    valueList.setText("distortion: 0/100 \ntype: 0 \ndry/wet 0/100 \n");
    addAndMakeVisible(valueList);
    
    
    // every png can have their own sample rate if we make the updater an object ig
    
    
    setSize (500, 200);
}

_consoleAudioProcessorEditor::~_consoleAudioProcessorEditor()
{
}

//==============================================================================
void _consoleAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.fillAll();
    //g.drawImage(walter, 0, 225, 75, 75, 0, 0, 75, 75);
    //g.drawImage(walter, 100, 100, 100, 100, 100, 100, 75, 75);
    //g.drawImage(walter, 0, 100, 100, 100, 0, 0, walter.getWidth(), walter.getHeight());
    g.drawImage(wiseOldMan, 0, 100, 100, 100, 0, 0, wiseOldMan.getWidth(), wiseOldMan.getHeight());
    g.drawImage(dj, 100, 130, 140, 90, 0, 0, dj.getWidth(), dj.getHeight());
    g.drawImage(puffle, 100, 0, 100, 100, 0, 0, puffle.getWidth(), puffle.getHeight());
    //g.drawImage(dj, 0, 0, 140, 90, 0, 0, dj.getWidth(), dj.getHeight());
}

void _consoleAudioProcessorEditor::resized()
{
    textEditor.setBounds(0, 0, 300, 100);
    consoleLog.setBounds(300,0,200,100);
    consoleLabel.setBounds(100, 100, 300, 100);
    valueList.setBounds(300,100,200,100);
}
