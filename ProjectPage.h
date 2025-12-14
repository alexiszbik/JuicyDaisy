/*
  ==============================================================================

    ProjectPage.h
    Created: 4 May 2025 5:17:10pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DaisyYMNK/DSP/ModuleCore.h"

#include "ModulePage.h"
#include "ParametersPage.h"

using namespace juce;
using namespace std;
using namespace ydaisy;

class ProjectPage : public Component, ChangeListener {
    
public:
    ProjectPage(ModuleCore*);
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    virtual void changeListenerCallback (ChangeBroadcaster* source) override;
    
private:
    ModuleCore* core;
    
    juce::TabbedComponent tabs { juce::TabbedButtonBar::TabsAtTop };
    
    std::unique_ptr<ModulePage> modulePage;
    std::unique_ptr<ParametersPage> parametersPage;
    
};
