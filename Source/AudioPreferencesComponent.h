/*
  ==============================================================================

    AudioPreferencesComponent.h
    Created: 19 Sep 2019 4:27:07pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "QLooperLookAndFeel.h"
//==============================================================================
/*
*/
class AudioPreferencesComponent : public Component
{
public:
    AudioPreferencesComponent(AudioDeviceManager& manager);
    ~AudioPreferencesComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    AudioDeviceSelectorComponent deviceSelector;
    std::unique_ptr<QLooperLookAndFeel> laf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPreferencesComponent)
};
