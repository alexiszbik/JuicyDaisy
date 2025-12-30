#pragma once

#include <JuceHeader.h>

#define TEST_APP 1

#include "ProjectPage.h"

class Project {
public:
    Project(juce::String name, ModuleCore* core) : page(new ProjectPage(core)), name(name), core(core) {
    }
    
    std::unique_ptr<ProjectPage> page;
    juce::String name;
    ModuleCore* core;
};

class ConsoleDisplayManager : public IDisplayManager {
public:
    void Write(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr, bool now=false) override {
        
    }
    virtual void WriteLine(uint8_t lineIdx, const char* str)  override {
        
    }
    virtual void WriteNow(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr) override {
        
    }
};

class TestModuleComponent  : public juce::AudioAppComponent, public juce::MidiInputCallback, public juce::ChangeListener
{
public:
    //==============================================================================
    TestModuleComponent(std::vector<Project*>projects);
    ~TestModuleComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    //==============================================================================
    virtual void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    virtual void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
private:
    vector<std::unique_ptr<juce::MidiInput>> midiInputs;
    
    juce::TabbedComponent tabs { juce::TabbedButtonBar::TabsAtTop };
    
    ModuleCore* core;

    vector<Project*> projects;

    int outChannelCount = 2;
    
    ConsoleDisplayManager consoleDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestModuleComponent)
};
