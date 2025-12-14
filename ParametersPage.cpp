/*
  ==============================================================================

    ParametersPage.cpp
    Created: 4 May 2025 4:57:10pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#include "ParametersPage.h"

ParametersPage::ParametersPage(ModuleCore* moduleCore) : core(moduleCore) {
    
    auto params = core->getAllParameters();
    for (auto& param : params) {
        
        Slider* slider = new Slider();
        slider->setRange (0, 1);
        slider->addListener (this);
        slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, 60, 20);
        
        components.push_back(slider);
        addAndMakeVisible(slider);
        
        labels.push_back(new Label("", juce::String(param->getName())));
        addAndMakeVisible(labels.back());
    }
    
    addAndMakeVisible(dumpButton);
    
    dumpButton.onClick = [this] {
        core->dumpAllParameters();
    };
    
}

void ParametersPage::updateHIDValues() {
        Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 100); //fix a weird bug when loading of preset and udpated UI are not synced ...
        
        int index = 0;
        for (auto& component : components) {
            
            float value = core->getDSPValue(index);
            
            auto slider = dynamic_cast<Slider*>(component);
            if (slider) {
                slider->setValue(value, dontSendNotification);
            }
            index++;
        }

}

ParametersPage::~ParametersPage() {
    for (auto& component : components) {
        delete component;
    }
    
    for (auto& label : labels) {
        delete label;
    }
}

void ParametersPage::paint (juce::Graphics& g) {
   
}

void ParametersPage::resized() {
    
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
    dumpButton.setBounds(x, y, 60, 30);
    
    updateHIDValues();
}


void ParametersPage::sliderValueChanged (juce::Slider* slider) {
    float value = slider->getValue();
    int index = getComponentIndex(slider);
    core->setDSPValue(index, value);
}


