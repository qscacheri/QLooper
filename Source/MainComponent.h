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

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public MenuBarModel::Listener
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
    
    virtual void menuBarItemsChanged (MenuBarModel *menuBarModel) override;
    virtual void menuCommandInvoked (MenuBarModel *menuBarModel, const ApplicationCommandTarget::InvocationInfo &info) override;
    virtual void menuBarActivated (MenuBarModel *menuBarModel, bool isActive) override;

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
            setVisible (true);
            
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
    std::unique_ptr<AudioWindow> audioWindow;
private:
    std::unique_ptr<QLooperMenuBarModel> model;
    
    std::unique_ptr<TwoPartSVGKnob> knob;
    std::unique_ptr<ImageButton> playButton;
    std::unique_ptr<ImageButton> recordButton;

    std::unique_ptr<TextButton> ioButton;
    Playhead playhead;
    Looper looper;
    ToneGeneratorAudioSource tone;
    Metronome metronome;
    
    //components
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
