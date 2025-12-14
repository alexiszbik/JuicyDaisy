/*
  ==============================================================================

    ParametersPage.h
    Created: 4 May 2025 4:57:10pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DaisyYMNK/DSP/ModuleCore.h"

using namespace juce;
using namespace std;
using namespace ydaisy;

class ParametersPage : public Component, juce::Slider::Listener {
public:
    ParametersPage(ModuleCore*);
    virtual ~ParametersPage();
    
public:
    
    int getComponentIndex(Component* component) {
        auto it = find(components.begin(), components.end(), component);
      
        // If element was found
        if (it != components.end())
        {
            int index = (int)(it - components.begin());
            return index;
        }
        return -1;
    }
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //==============================================================================
    virtual void sliderValueChanged (juce::Slider* slider) override;
    
    void updateHIDValues();
    
private:
    ModuleCore* core;
    
    vector<Component*> components;
    vector<Component*> labels;
    
    TextButton dumpButton = TextButton("Dump", "Dump");
    
    const int topTab = 40;
    const int componentHeight = 26;

};
