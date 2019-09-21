#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    audioWindow.reset(new AudioWindow("Audio Preferences", deviceManager));
    
    model.reset(new QLooperMenuBarModel(this));
    PopupMenu appleMenu;
    appleMenu.addItem(QLooperMenuBarModel::ids::AudioPreferencesId, "Audio Setup");
    MenuBarModel::setMacMainMenu(model.get(), &appleMenu);
    model->addListener(this);
    
    Image knobImage(ImageCache::getFromMemory(BinaryData::knob_base_png, BinaryData::knob_base_pngSize));
    Image tickImage(ImageCache::getFromMemory(BinaryData::tick_png, BinaryData::tick_pngSize));
    
    knob.reset(new TwoPartSVGKnob(knobImage, tickImage));
    addAndMakeVisible(knob.get());
    
    Image buttonOnUp = ImageCache::getFromMemory(BinaryData::button_on_up_png, BinaryData::button_on_up_pngSize);
    Image buttonOnDown = ImageCache::getFromMemory(BinaryData::button_on_down_png, BinaryData::button_on_down_pngSize);
    Image buttonOffUp = ImageCache::getFromMemory(BinaryData::button_off_up_png, BinaryData::button_off_up_pngSize);
    Image buttonOffDown = ImageCache::getFromMemory(BinaryData::button_off_down_png, BinaryData::button_off_down_pngSize);
    
    inputMonitoringButton.reset(new TextButton());
    inputMonitoringButton->setButtonText("Input Monitoring");
    addAndMakeVisible(inputMonitoringButton.get());
    inputMonitoringButton->onClick = [this] {looper.setInputMonitoring(!looper.getInputMonitoring()); };
    
    metroButton.reset(new TextButton());
    metroButton->setButtonText("Metronome on/off");
    addAndMakeVisible(metroButton.get());
    metroButton->onClick = [this] {looper.setMetronomeEnabled(!looper.getMetronomeEnabled());};
    
    recordButton.reset(new ImageToggleButton());
    recordButton->setImages(buttonOffUp, buttonOffDown, buttonOnUp, buttonOnDown);
    recordButton->onClick = [this] {
        if (looper.isRecording)
            looper.stopRecording();
        else
            looper.startRecording();
    };
    addAndMakeVisible(recordButton.get());
    
    clearButton.reset(new ImageButton());
    clearButton->setImages(false, true, true, buttonOffUp, 1.f, Colours::transparentBlack, buttonOffUp, 1.f, Colours::transparentBlack, buttonOffDown, 1.f, Colours::transparentBlack);
    clearButton->onClick = [this] { looper.clearLoop(); };
    addAndMakeVisible(clearButton.get());
    
    
    tempoSlider.reset(new Slider());
    tempoSlider->setRange(40, 240, 1);
    tempoSlider->setSliderStyle(Slider::SliderStyle::Rotary);
    tempoSlider->setValue(looper.getTempo());
    tempoSlider->addListener(this);
    addAndMakeVisible(tempoSlider.get());
    
    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    
    setSize (800, 600);
    
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
}

MainComponent::~MainComponent()
{
    
    File file("~/Library/Application Support/QLooper/io.xml");
    DBG(file.getFileName());
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
    
    looper.getNextAudioBlock(*bufferToFill.buffer);

    
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.drawImageWithin(backgroundImage, JUCE_LIVE_CONSTANT(0),
                      JUCE_LIVE_CONSTANT(0), getWidth(), getHeight(), RectanglePlacement::yTop);
}

void MainComponent::resized()
{
    Rectangle<int> area = getLocalBounds();
    area.removeFromRight(getWidth() / 2);
    area.removeFromBottom(getHeight() / 2);
    recordButton->setBounds(area);
    
    area.translate(0, getHeight() / 2);
    area = area.withSizeKeepingCentre(getHeight() / 8, getHeight() / 8);
    clearButton->setBounds(area);
    
    area = getLocalBounds().removeFromRight(getWidth() / 2);
    area = area.removeFromTop(getHeight() / 3);
    inputMonitoringButton->setBounds(area);
    
    area.translate(0, getHeight() / 3);
    metroButton->setBounds(area);
    
    area.translate(0, getHeight() / 3);
    tempoSlider->setBounds(area);
    
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
}
