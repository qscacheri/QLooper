/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Playhead.h"
#include "Looper.h"
#include "Metronome.h"
#include "TwoPartSVGKnob.h"
#include "QLooperMenuBar.h"
#include "AudioPreferencesComponent.h"
#include "ImageToggleButton.h"
#include "LevelMeter.h"
#include <array>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public MenuBarModel::Listener, public Slider::Listener, public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    float getInputLevel();
    float getOutputLevel();

    virtual void menuBarItemsChanged (MenuBarModel *menuBarModel) override;
    virtual void menuCommandInvoked (MenuBarModel *menuBarModel, const ApplicationCommandTarget::InvocationInfo &info) override;
    virtual void menuBarActivated (MenuBarModel *menuBarModel, bool isActive) override;
    
    //Slider Listener
    virtual void sliderValueChanged(Slider *slider) override;

    // command target
    virtual ApplicationCommandTarget* getNextCommandTarget();
    virtual void getAllCommands (Array< CommandID > &commands);
    virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo &result);
    virtual bool perform (const InvocationInfo &info);
    
    
private:
    
    class AudioWindow : public DocumentWindow, public ApplicationCommandTarget
    {
    public:
        AudioWindow (String name, AudioDeviceManager& manager)  : DocumentWindow (name,
                                                     Desktop::getInstance().getDefaultLookAndFeel()
                                                     .findColour (ResizableWindow::backgroundColourId),
                                                     DocumentWindow::allButtons)
        {
            centreWithSize (getWidth(), getHeight());
            setUsingNativeTitleBar (true);
            setContentOwned (new AudioPreferencesComponent(manager), true);
            setResizable (true, true);
            setVisible (false);
            
        }
        
        void closeButtonPressed() override
        {
            setVisible(false);
        }
        
        // application target methods
        virtual ApplicationCommandTarget * getNextCommandTarget () override
        {
            return this;
        }
        
        virtual void getAllCommands (Array< CommandID > &commands) override
        {
            
        }
        
        virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo &result) override
        {
            
        }
        
        virtual bool perform (const InvocationInfo &info) override
        {
            return true;
        }
        
    };
public:
    std::unique_ptr<KeyMappingEditorComponent> keyMapper;

    std::unique_ptr<AudioWindow> audioWindow;
private:
    std::unique_ptr<QLooperMenuBarModel> model;
    
    std::unique_ptr<Slider> inputLevelSlider;
    std::unique_ptr<Slider> outputLevelSlider;
    
    std::unique_ptr<LevelMeter> inputMeter;
    std::unique_ptr<LevelMeter> outputMeter;

    std::unique_ptr<TwoPartSVGKnob> knob;
    std::unique_ptr<ImageButton> playButton;
    std::unique_ptr<ImageToggleButton> recordButton;
    std::unique_ptr<ImageButton> clearButton;

    std::unique_ptr<TextButton> inputMonitoringButton;
    std::unique_ptr<ImageButton> metroButton;
    std::unique_ptr<Slider> tempoSlider;
    std::unique_ptr<TextButton> ioButton;
    
    std::array<std::unique_ptr<LevelMeter>, 3> meters;
    std::array<std::unique_ptr<ImageToggleButton>, 3> recordButtons;
    std::array<std::unique_ptr<ImageButton>, 3> clearButtons;
    Image backgroundImage;
    Image divider;
    
    Looper looper;
    ToneGeneratorAudioSource tone;
    
    float inputLevel = 0;
    float outputLevel = 0;
    
    Rectangle<int> masterControlsRect;
    //components
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
