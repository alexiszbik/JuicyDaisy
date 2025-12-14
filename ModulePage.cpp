/*
  ==============================================================================

 ModulePage.cpp
    Created: 4 May 2025 4:46:11pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "ModulePage.h"

ModulePage::ModulePage(ModuleCore* moduleCore) : core(moduleCore) {
    
    auto desc = core->getHIDDescription();
    int index = 0;
    for (auto& hidElement : desc) {
        
        Component* component;
        
        switch (hidElement.type) {
            case HIDType::kCVInput :
            case HIDType::kKnob :
                {
                    juce::Slider* slider = new juce::Slider();
                    slider->setRange (0, 1);
                    slider->addListener (this);
                    component = slider;
                }
                break;
                
            case HIDType::kRotary8 :
                {
                    juce::Slider* slider = new juce::Slider();
                    slider->setRange (0, 1, (1./7.));
                    slider->addListener (this);
                    component = slider;
                }
                break;
                
            case HIDType::kSwitch :
                {
                    juce::ToggleButton* button = new juce::ToggleButton();
                    button->addListener(this);
                    component = button;
                }
                break;
                
            case HIDType::kGateInput :
            case HIDType::kButton :
                {
                    juce::TextButton* button = new juce::TextButton("");
                    button->setButtonText("PUSH");
                    button->addListener(this);
                    component = button;
                }
                break;
            case HIDType::kLed :
                continue;;
            default:
                break;
        }
        
        components.push_back(component);
        addAndMakeVisible(component);
        auto label = new Label("", juce::String(hidElement.name));
        labels.push_back(label);
        addAndMakeVisible(label);
        index++;
    }
}

ModulePage::~ModulePage() {
    for (auto& component : components) {
        delete component;
    }
    
    for (auto& label : labels) {
        delete label;
    }
}


void ModulePage::sliderValueChanged (juce::Slider* slider) {
    float value = slider->getValue();
    int index = getComponentIndex(slider);
    core->setHIDValue(index, value);
}
    
void ModulePage::buttonStateChanged (Button* button) {
    auto toggleButton = dynamic_cast<ToggleButton*>(button);
    int index = getComponentIndex(button);
    if (toggleButton) {
        core->setHIDValue(index, button->getToggleStateValue().getValue());
    }
}

void ModulePage::buttonClicked (Button* button) {
    int index = getComponentIndex(button);
    core->setHIDValue(index, button->isDown());
}

void ModulePage::paint (juce::Graphics& g) {
    
}

void ModulePage::resized() {
    Component* previousComponent = nullptr;
    
    int componentCount = (int)components.size();
    
    int totalHeight = (componentCount * componentHeight) + 30;
    int height = (getHeight() - topTab);
    
    int columnCount = ceil((float)totalHeight / (float)height);
    
    const int labelWidth = 80;
    
    int y = topTab;
    int x = 0;
    int w = getWidth()/columnCount;
    int h = componentHeight;
    for (int i = 0; i < componentCount ; i++) {
        if (previousComponent) {
            if (previousComponent->getBottom() + h > getHeight()) {
                x += w;
                y = topTab;
            }
        }
        
        for (auto& component : {labels.at(i), components.at(i)}) {
            
            if (dynamic_cast<Label*>(component)) {
                component->setBounds(x, y, labelWidth, h);
            } else {
                component->setBounds(x + labelWidth, y, w - labelWidth, h);
                y = previousComponent ? previousComponent->getBottom() : 0;
                
            }
            previousComponent = component;
        }
    }
}
