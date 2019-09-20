#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    audioWindow.reset(new AudioWindow("Audio Preferences", deviceManager));
    
    model.reset(new QLooperMenuBarModel(this));
    MenuBarModel::setMacMainMenu(model.get());
    model->addListener(this);
    
    metronome.playhead = &playhead;
    
    Image knobImage(ImageCache::getFromMemory(BinaryData::knob_base_png, BinaryData::knob_base_pngSize));
    Image tickImage(ImageCache::getFromMemory(BinaryData::tick_png, BinaryData::tick_pngSize));
    
    knob.reset(new TwoPartSVGKnob(knobImage, tickImage));
    addAndMakeVisible(knob.get());
    
    Image buttonUp = ImageCache::getFromMemory(BinaryData::button_up_png, BinaryData::button_up_pngSize);
    
    Image buttonDown = ImageCache::getFromMemory(BinaryData::button_down_png, BinaryData::button_down_pngSize);

    recordButton.reset(new ImageButton());
    recordButton->setImages(false, true, true, buttonUp, 1.f, Colours::transparentBlack, buttonUp, 1.f, Colours::transparentBlack, buttonDown, 1.f, Colours::transparentBlack);
    recordButton->onClick = [this] {
        if (looper.isRecording)
            looper.stopRecording();
        else
            looper.startRecording();
    };
    addAndMakeVisible(recordButton.get());
    
    ioButton.reset(new TextButton());
    ioButton->setButtonText("i/o");
    
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
        deviceManager.initialise(1, 1, io.get(), true);
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
    
    metronome.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    looper.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    tone.prepareToPlay(samplesPerBlockExpected, sampleRate);
    tone.setAmplitude(.5);
    tone.setFrequency(440);
    
    playhead = Playhead(sampleRate);
    looper.playhead = &playhead;
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    AudioBuffer<float> bufferCopy(1, bufferToFill.buffer->getNumSamples());

    AudioSourceChannelInfo copy(&bufferCopy, 0, bufferCopy.getNumSamples());

    metronome.getNextBlock(copy);

    // update playhead position
    playhead.updatePosition(bufferToFill.buffer->getNumSamples());

    looper.getNextAudioBlock(*bufferToFill.buffer);

    bufferToFill.buffer->addFrom(0, 0, *copy.buffer, 0, 0, bufferToFill.buffer->getNumSamples());
    
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    recordButton->setBounds(10, 10, 400, 400);
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
