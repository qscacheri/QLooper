/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : deviceSelector(deviceManager, 1, 1, 1, 1, true, true, false, false)
{
    
    
    addAndMakeVisible(deviceSelector);
    
    metronome.playhead = &playhead;
    
    recordButton.reset(new TextButton());
    recordButton->setButtonText("Record");
    recordButton->onClick = [this] {
        if (looper.isRecording)
            looper.stopRecording();
        else
            looper.startRecording();
    };
    addAndMakeVisible(recordButton.get());
    
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
    DBG(io->toString());
    io->writeTo(file);

//    io.release();
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    metronome.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
//    looper.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    tone.prepareToPlay(samplesPerBlockExpected, sampleRate);
    tone.setAmplitude(.5);
    tone.setFrequency(440);
    
    playhead = Playhead(sampleRate);
    looper.playhead = &playhead;
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // update playhead position
    playhead.updatePosition(bufferToFill.buffer->getNumSamples());
    
    looper.getNextAudioBlock(*bufferToFill.buffer);

    
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
    recordButton->setBounds(getLocalBounds().removeFromLeft(getWidth() / 2));
    deviceSelector.setBounds(getLocalBounds().removeFromRight(getWidth() / 2));

}
