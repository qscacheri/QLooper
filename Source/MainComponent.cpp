#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    ValueTree data
    { "APPLICATION_DATA", {},
        {
            { "MASTER_DATA", {},
                {
                    { "TEMPO", {{ "LOCKED", true }, { "BPM", 120}}},
                    { "LEVEL", {{ "IN_GAIN", 1.f }, { "OUT_GAIN", 1.f}, { "LENGTH", 64}}}
                }
            },
            { "LOOP", {{ "INDEX", 1 }, { "GAIN", 1.f}, { "ACTIVE", true }}},
            { "LOOP", {{ "INDEX", 2 }, { "GAIN", 1.f}, { "ACTIVE", true }}},
            { "LOOP", {{ "INDEX", 3 }, { "GAIN", 1.f}, { "ACTIVE", true }}},
        }
    };
    
    applicationData = ValueTree(data);
    
    audioWindow.reset(new AudioWindow("Audio Preferences", deviceManager));
    
    Image knobImage(ImageCache::getFromMemory(BinaryData::knob_base_png, BinaryData::knob_base_pngSize));
    Image tickImage(ImageCache::getFromMemory(BinaryData::tick_png, BinaryData::tick_pngSize));
    
    knob.reset(new TwoPartSVGKnob(knobImage, tickImage));
    addAndMakeVisible(knob.get());
    
    Image buttonOnUp = ImageCache::getFromMemory(BinaryData::input_button_on_up_png, BinaryData::input_button_on_up_pngSize);
    Image buttonOnDown = ImageCache::getFromMemory(BinaryData::input_button_on_down_png, BinaryData::input_button_on_down_pngSize);
    Image buttonOffUp = ImageCache::getFromMemory(BinaryData::input_button_off_up_png, BinaryData::input_button_off_up_pngSize);
    Image buttonOffDown = ImageCache::getFromMemory(BinaryData::input_button_off_down_png, BinaryData::input_button_off_down_pngSize);
    
    inputMonitoringButton.reset(new ImageToggleButton());
    inputMonitoringButton->setImages(buttonOffUp, buttonOffDown, buttonOnUp, buttonOnDown);
    addAndMakeVisible(inputMonitoringButton.get());
    inputMonitoringButton->onClick = [this] {looper.setInputMonitoring(!looper.getInputMonitoring()); };
    
    metroButton.reset(new ImageButton());
    addAndMakeVisible(metroButton.get());
    Image metroButtonUp = ImageCache::getFromMemory(BinaryData::metro_button_up_png, BinaryData::metro_button_up_pngSize);
    Image metroButtonDown = ImageCache::getFromMemory(BinaryData::metro_button_down_png, BinaryData::metro_button_down_pngSize);

    metroButton->setImages(false, true, true, metroButtonUp, 1.f, Colours::transparentBlack, metroButtonUp, 1.f, Colours::transparentBlack, metroButtonDown, 1.f, Colours::transparentBlack);
    metroButton->onClick = [this] {looper.setMetronomeEnabled(!looper.getMetronomeEnabled());};
    
    tempoLockButton.reset(new ImageButton());
    tempoLockButton->setImages(false, true, true, metroButtonUp, 1.f, Colours::transparentBlack, metroButtonUp, 1.f, Colours::transparentBlack, metroButtonDown, 1.f, Colours::transparentBlack);
    tempoLockButton->onClick = [this] { looper.toggleTempoLock(); };
    addAndMakeVisible(tempoLockButton.get());
    
    tempoSlider.reset(new Slider());
    tempoSlider->setRange(40, 240, 1);
    tempoSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    tempoSlider->setValue(looper.getTempo());
    tempoSlider->addListener(this);
    addAndMakeVisible(tempoSlider.get());
    
    auto getInput = [this] { return getInputLevel(); };
    LevelMeter::Options options;
    options.borderThicknessRatio = 0;
    inputMeter.reset(new LevelMeter(getInput));
    addAndMakeVisible(inputMeter.get());
    inputMeter->setColour(LevelMeter::ColourIds::meterColourId, Colour(0xff22ff00));
    inputMeter->setOptions(options);

    auto getOutput = [this] { return getOutputLevel(); };
    outputMeter.reset(new LevelMeter(getOutput));
    addAndMakeVisible(outputMeter.get());
    outputMeter->setColour(LevelMeter::ColourIds::meterColourId, Colour(0xff22ff00));
    outputMeter->setOptions(options);

    buttonOnUp = ImageCache::getFromMemory(BinaryData::button_on_up_png, BinaryData::button_on_up_pngSize);
    buttonOnDown = ImageCache::getFromMemory(BinaryData::button_on_down_png, BinaryData::button_on_down_pngSize);
    buttonOffUp = ImageCache::getFromMemory(BinaryData::button_off_up_png, BinaryData::button_off_up_pngSize);
    buttonOffDown = ImageCache::getFromMemory(BinaryData::button_off_down_png, BinaryData::button_off_down_pngSize);

    
    for (int i = 0; i < 3; i++){
        
        // meters

        // record buttons
        recordButtons[i].reset(new ImageToggleButton());
        recordButtons[i]->getProperties().set("loopNumber", i);
        recordButtons[i]->setImages(buttonOffUp, buttonOffDown, buttonOnUp, buttonOnDown);
        recordButtons[i]->addListener(this);
        addAndMakeVisible(recordButtons[i].get());
        
        // play buttons
        playButtons[i].reset(new ImageButton());
        playButtons[i]->setImages(false, true, true, buttonOffUp, 1.f, Colours::transparentBlack, buttonOffUp, 1.f, Colours::transparentBlack, buttonOffDown, 1.f, Colours::transparentBlack);
        addAndMakeVisible(playButtons[i].get());
        
        // clear buttons
        clearButtons[i].reset(new ImageButton());
        clearButtons[i]->setImages(false, true, true, buttonOffUp, 1.f, Colours::transparentBlack, buttonOffUp, 1.f, Colours::transparentBlack, buttonOffDown, 1.f, Colours::transparentBlack);
        addAndMakeVisible(clearButtons[i].get());
        
        // level knobs
        loopGainKnobs[i].reset(new TwoPartSVGKnob(knobImage, tickImage));
        loopGainKnobs[i]->addListener(this);
        loopGainKnobs[i]->setRange(0, 1);
        addAndMakeVisible(loopGainKnobs[i].get());

    }
    
    playButtons[0]->onClick = [this] { looper.setActive(0, !looper.getActive(0)); };
    playButtons[1]->onClick = [this] { looper.setActive(1, !looper.getActive(1)); };
    playButtons[2]->onClick = [this] { looper.setActive(2, !looper.getActive(2)); };

    // callback assignments
    clearButtons[0]->onClick = [this] { looper.clearLoop(0); };
    clearButtons[1]->onClick = [this] { looper.clearLoop(1); };
    clearButtons[2]->onClick = [this] { looper.clearLoop(2); };
    
//  meters
    meters[0].reset(new LevelMeter([this] (){ return looper.getLoopLevel(0); }));
    meters[0]->setColour(LevelMeter::ColourIds::meterColourId, Colour(0xff22ff00));
    meters[0]->setOptions(options);
    addAndMakeVisible(meters[0].get());

    meters[1].reset(new LevelMeter([this] (){ return looper.getLoopLevel(1); }));
    meters[1]->setColour(LevelMeter::ColourIds::meterColourId, Colour(0xff22ff00));
    meters[1]->setOptions(options);
    addAndMakeVisible(meters[1].get());

    meters[2].reset(new LevelMeter([this] (){ return looper.getLoopLevel(2); }));
    meters[2]->setColour(LevelMeter::ColourIds::meterColourId, Colour(0xff22ff00));
    meters[2]->setOptions(options);
    addAndMakeVisible(meters[2].get());

    

    divider = ImageCache::getFromMemory(BinaryData::divider_png, BinaryData::divider_pngSize);

    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    
    setSize (1200, 900);
    
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    File f("~/Library/Application Support/QLooper/io.xml");
    if (f.exists())
    {
        std::unique_ptr<XmlElement> io = XmlDocument::parse(f);
        deviceManager.initialise(1, 2, io.get(), true);
    }
    
    deviceManager.addMidiInputDeviceCallback("", this);
}

MainComponent::~MainComponent()
{
    File file("~/Library/Application Support/QLooper/io.xml");
    std::unique_ptr<XmlElement> io = deviceManager.createStateXml();
    if (io.get() != nullptr)
        io->writeTo(file);
    
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    looper.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    tone.prepareToPlay(samplesPerBlockExpected, sampleRate);
    tone.setAmplitude(.5);
    tone.setFrequency(440);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    inputLevel = bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.numSamples);
    
    looper.getNextAudioBlock(*bufferToFill.buffer);

    outputLevel = bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.numSamples);

    
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    int w = getWidth() * .01;
    int h = .8 * (getHeight() - masterControlsRect.getHeight());
    int x = getWidth() / 3;
    int y = masterControlsRect.getHeight() * 1.2;
    
    g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::yTop);
    g.drawImageWithin(divider, x, y, w, h, RectanglePlacement::stretchToFit);
    x += getWidth() / 3;
    g.drawImageWithin(divider, x, y, w, h, RectanglePlacement::stretchToFit);

}

void MainComponent::resized()
{
    auto inputs = MidiInput::getDevices();
    DBG(inputs[1]);

    Rectangle<int> area = getLocalBounds();

    //master controls
    masterControlsRect = getLocalBounds().removeFromTop(getHeight() * .25);
    
    area = masterControlsRect;
    area.removeFromRight(area.getWidth() * .96);
    area.translate(area.getWidth() / 2, area.getWidth() / 2);
    inputMeter->setBounds(area);
    
    area.setX(getWidth() - inputMeter->getWidth());
    area.translate(-area.getWidth() / 2, 0);
    outputMeter->setBounds(area);
    
    area = masterControlsRect;
    area = area.withSizeKeepingCentre(getWidth() / 8, masterControlsRect.getHeight() * .4);
    metroButton->setBounds(area);
    
    area.translate(-100, 0);
    tempoLockButton->setBounds(area);
    
    area = getLocalBounds();
    area.removeFromTop(masterControlsRect.getHeight());
    area = area.removeFromLeft(getWidth() / 3);
    area.removeFromBottom(getHeight() / 2);
    area = area.withSizeKeepingCentre(area.getHeight(), area.getHeight());
    recordButtons[0]->setBounds(area);
    
    for (int i = 1; i <= 2; i++){
        area.translate(getWidth() / 3, 0);
        recordButtons[i]->setBounds(area);
    }
    
    area = recordButtons[0]->getBounds();
    area.translate(0, recordButtons[0]->getHeight() * 1.1);
    area = area.withSizeKeepingCentre(area.getWidth() * .75, area.getWidth() * .75);
    loopGainKnobs[0]->setBounds(area);
    
    for (int i = 1; i <= 2; i++){
        area.translate(getWidth() / 3, 0);
        loopGainKnobs[i]->setBounds(area);
    }

    area = recordButtons[0]->getBounds();
    area.translate(-area.getWidth() * .6, 0);
    area = area.withSizeKeepingCentre(area.getWidth() * .5, area.getWidth() * .5);
    clearButtons[0]->setBounds(area);
    
    for (int i = 1; i <= 2; i++){
        area.translate(getWidth() / 3, 0);
        clearButtons[i]->setBounds(area);
    }


    area = recordButtons[0]->getBounds();
    area = area.withWidth(.9 * (getWidth() / 3));
    area = area.withHeight(inputMeter->getWidth());
    area.translate(0, 500);
    area.setCentre(recordButtons[0]->getBounds().getCentreX(), area.getY());
    meters[0]->setBounds(area);
    
    for (int i = 1; i <= 2; i++){
        area.translate(getWidth() / 3, 0);
        meters[i]->setBounds(area);
    }
    
    area = getLocalBounds().removeFromRight(getWidth() / 2);
    area = area.removeFromTop(getHeight() / 3);
    inputMonitoringButton->setBounds(area);
    
    area.translate(0, getHeight() / 3);
    
    area.translate(0, getHeight() / 3);
//    tempoSlider->setBounds(area);
    
}

//menu bar listener methods
void MainComponent::menuBarItemsChanged (MenuBarModel *menuBarModel)
{
    
}

void MainComponent::menuCommandInvoked (MenuBarModel *menuBarModel, const ApplicationCommandTarget::InvocationInfo &info)
{
    
}

void MainComponent::menuBarActivated (MenuBarModel *menuBarModel, bool isActive)
{
    
}

void MainComponent::sliderValueChanged(Slider *slider)
{
    if (slider == tempoSlider.get())
    {
        looper.setTempo(slider->getValue());
    }
    
    if (slider == loopGainKnobs[0].get())
        looper.setLoopGain(0, slider->getValue());
    
    if (slider == loopGainKnobs[1].get())
        looper.setLoopGain(1, slider->getValue());

    if (slider == loopGainKnobs[2].get())
        looper.setLoopGain(2, slider->getValue());

}

float MainComponent::getInputLevel()
{
    return inputLevel;
}

float MainComponent::getOutputLevel()
{
    return outputLevel;
}

// command target
ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    
}

void MainComponent::getAllCommands (Array< CommandID > &commands)
{
    commands.add(1);
}

void MainComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo &result)
{
    result.setInfo("Preferences", "Preferences", "Utility", 0);
}

bool MainComponent::perform (const InvocationInfo &info)
{
    audioWindow->setVisible(true);
    audioWindow->toFront(true);
    return true;
}
// button listener methods
void MainComponent::buttonClicked(Button* b)
{
    NamedValueSet properties = b->getProperties();
    if (properties.contains("loopNumber"))
    {
        int index = properties["loopNumber"];
        looper.toggleRecording(index);
    }
}

void MainComponent::buttonStateChanged(Button* b)
{
    
}

// midicallback functions
void MainComponent::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
{
    if (message.isProgramChange())
    {
        int num = message.getProgramChangeNumber();
        switch (num) {
            case 0:
                looper.toggleRecording(0);
                MessageManager::callAsync([this]() { recordButtons[0]->setToggleState(looper.isRecording, dontSendNotification); });
                break;
            case 1:
                looper.toggleRecording(1);
                MessageManager::callAsync([this]() { recordButtons[1]->setToggleState(looper.isRecording, dontSendNotification); });
                break;
            case 2:
                looper.toggleRecording(2);
                MessageManager::callAsync([this]() { recordButtons[2]->setToggleState(looper.isRecording, dontSendNotification); });
                break;
            default:
                break;
        }
    }
}

void MainComponent::handlePartialSysexMessage (MidiInput *source, const uint8 *messageData, int numBytesSoFar, double timestamp)
{
    
}
