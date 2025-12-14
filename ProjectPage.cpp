/*
  ==============================================================================

    ProjectPage.cpp
    Created: 4 May 2025 5:17:10pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "ProjectPage.h"

ProjectPage::ProjectPage(ModuleCore* moduleCore) :
core(moduleCore),
modulePage(new ModulePage(core)),
parametersPage(new ParametersPage(core))
{
    addAndMakeVisible(tabs);

    tabs.addTab("Project", juce::Colours::darkblue, modulePage.get(), true);
    tabs.addTab("Parameters", juce::Colours::darkgreen, parametersPage.get(), true);
    
    tabs.getTabbedButtonBar().addChangeListener (this);
    
}

void ProjectPage::paint (juce::Graphics& g) {
    
}

void ProjectPage::resized() {
    tabs.setBounds(getLocalBounds());
}

void ProjectPage::changeListenerCallback (ChangeBroadcaster* source) {
    parametersPage->updateHIDValues();
}
