/*
  ==============================================================================

    ModulePage.h
    Created: 4 May 2025 4:20:24pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DaisyYMNK/DSP/ModuleCore.h"

using namespace juce;
using namespace std;
using namespace ydaisy;

class ModulePage : public Component, juce::Slider::Listener, juce::Button::Listener {
public:
    ModulePage(ModuleCore*);
    virtual ~ModulePage();
    
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
    virtual void buttonStateChanged (juce::Button* button) override;
    virtual void buttonClicked (juce::Button* button) override;
    
private:
    ModuleCore* core;
    
    vector<Component*> components;
    vector<Component*> labels;
    
    const int topTab = 40;
    const int componentHeight = 26;

};
