#include "JDMainComponent.h"

//==============================================================================
JDMainComponent::JDMainComponent()
{
    addAndMakeVisible(tabs);
    
    projects.push_back(new Project("PolyFM", &polyFm));
    
    for (auto& project : projects) {
        tabs.addTab(project->name, juce::Colours::darkblue, project->page.get(), true);
    }
    
    core = projects.front()->core;
    
    tabs.getTabbedButtonBar().addChangeListener(this);

    setSize (800, 600);

    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, outChannelCount);
    }
    
    auto setup = deviceManager.getAudioDeviceSetup();
    setup.bufferSize = 64; // ou une autre valeur supportée par le device
    deviceManager.setAudioDeviceSetup(setup, true);
    
    juce::String virtualMidiName = "DaisyTest";
    midiInputs.push_back(juce::MidiInput::createNewDevice(virtualMidiName, this));
    
    auto availableInputs = juce::MidiInput::getAvailableDevices();
    for (auto& input : availableInputs)
    {
        if (!input.name.containsIgnoreCase(virtualMidiName))
        {
            midiInputs.push_back(juce::MidiInput::openDevice(input.identifier, this));
            break;
        }
    }
    
    for (auto& input : midiInputs) {
        input->start();
    }
}


JDMainComponent::~JDMainComponent()
{
    for (auto& input : midiInputs) {
        input->stop();
    }
    shutdownAudio();
}


//==============================================================================
void JDMainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    for (auto& project : projects) {
        project->core->init(outChannelCount, sampleRate);
    }
    
}

void JDMainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &tabs.getTabbedButtonBar())
    {
        int selectedIndex = tabs.getCurrentTabIndex();
        core = projects.at(selectedIndex)->core;
    }
}

void JDMainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
    
    if (message.isNoteOn()) {
        core->processMIDI(MIDIMessageType::kNoteOn, message.getChannel(), message.getNoteNumber(), message.getVelocity());
    } else if (message.isNoteOff()) {
        core->processMIDI(MIDIMessageType::kNoteOff, message.getChannel(), message.getNoteNumber(), message.getVelocity());
    } else if (message.isController()) {
        core->processMIDI(MIDIMessageType::kControlChange, message.getChannel(), message.getControllerNumber(), message.getControllerValue());
    } else if (message.isPitchWheel()) {
        core->processMIDI(MIDIMessageType::kPitchBend, message.getChannel(), message.getPitchWheelValue(), 0);
    } else if (message.isProgramChange()) {
        core->processMIDI(MIDIMessageType::kProgramChange, message.getChannel(), message.getProgramChangeNumber(), 0);
    }
}

void JDMainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::ScopedNoDenormals noDenormals;
    
    bufferToFill.clearActiveBufferRegion();
    auto buffer = bufferToFill.buffer;
    
    float* data[buffer->getNumChannels()];
    
    for (int channel = 0; channel < buffer->getNumChannels(); ++channel)
    {
        data[channel] = buffer->getWritePointer (channel);
    }
    
    core->process(data, buffer->getNumSamples());
}

void JDMainComponent::releaseResources()
{
}

//==============================================================================
void JDMainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void JDMainComponent::resized()
{
    tabs.setBounds(getLocalBounds());
}




