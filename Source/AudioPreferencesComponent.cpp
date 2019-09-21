/*
  ==============================================================================

    AudioPreferencesComponent.cpp
    Created: 19 Sep 2019 4:27:07pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "AudioPreferencesComponent.h"

//==============================================================================
AudioPreferencesComponent::AudioPreferencesComponent(AudioDeviceManager& manager) : deviceSelector(manager, 1, 1, 1, 1, true, true, false, false)
{
    laf.reset(new QLooperLookAndFeel());
    
//    deviceSelector.setLookAndFeel(laf.get());
    addAndMakeVisible(deviceSelector);
    
    setSize(500, 500);
}

AudioPreferencesComponent::~AudioPreferencesComponent()
{
}

void AudioPreferencesComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
}

void AudioPreferencesComponent::resized()
{
    Component::resized();
    deviceSelector.setBounds(getLocalBounds());
}
