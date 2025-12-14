#pragma once

#include <JuceHeader.h>

#define TEST_APP 1

#include "ProjectPage.h"

#include "PolyFMCore.h"

class Project {
public:
    Project(juce::String name, ModuleCore* core) : page(new ProjectPage(core)), name(name), core(core) {
    }
    
    std::unique_ptr<ProjectPage> page;
    juce::String name;
    ModuleCore* core;
};

class JDMainComponent  : public juce::AudioAppComponent, public juce::MidiInputCallback, public juce::ChangeListener
{
public:
    //==============================================================================
    JDMainComponent();
    ~JDMainComponent() override;

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
    
    PolyFMCore polyFm;
    
    ModuleCore* core;

    vector<Project*> projects;

    int outChannelCount = 2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JDMainComponent)
};
