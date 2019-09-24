/*
 ==============================================================================
 
 Metronome.h
 Created: 16 Sep 2019 2:18:27pm
 Author:  Quin Scacheri
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Playhead.h"

class Metronome
{
public:
    Metronome(Playhead* ph)
    {
        playhead = ph;
        
        formatManager.registerBasicFormats();
        std::unique_ptr<MemoryInputStream> inputStream(new MemoryInputStream(BinaryData::click_wav, BinaryData::click_wavSize, false));
        
        auto* reader = formatManager.createReaderFor (inputStream.get());
        if (reader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
            transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            readerSource.reset (newSource.release());
            
        }
        
        inputStream.release();
    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        readerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
        readerSource->setLooping(false);
    }
    
    void getNextBlock(const AudioSourceChannelInfo& bufferToFill)
    {
        int numSamples = bufferToFill.numSamples;
        bool startedPlayback = false;
        
        for (int i = 0; i < 16; i++)
        {
            if (playhead->getPositionInSamples() + bufferToFill.buffer->getNumSamples() > playhead->beatsToSamples(i * 4) && playhead->getPositionInSamples() <= playhead->beatsToSamples(i * 4))
            {
                readerSource->setNextReadPosition(0);
                int position = (playhead->getPositionInSamples() + numSamples) - playhead->beatsToSamples(i * 4);
                AudioBuffer<float> bufferCopy(1, numSamples - position);
                readerSource->getNextAudioBlock(AudioSourceChannelInfo(bufferCopy));
                bufferCopy.applyGain(0, 0, numSamples - position, gain);
                bufferToFill.buffer->addFrom(0, position, bufferCopy, 0, 0, numSamples - position);
                bufferToFill.buffer->addFrom(1, position, bufferCopy, 0, 0, numSamples - position);
                
                startedPlayback = true;
                break;

            }
                
        }
        
        if (!startedPlayback)
        {
            AudioBuffer<float> bufferCopy(1, numSamples);
            readerSource->getNextAudioBlock(AudioSourceChannelInfo(bufferCopy));
            bufferCopy.applyGain(0, 0, numSamples, gain);
            bufferToFill.buffer->addFrom(0, 0, bufferCopy, 0, 0, numSamples);
            bufferToFill.buffer->addFrom(1, 0, bufferCopy, 0, 0, numSamples);

        }

    }
    
    void setEnabled(bool enabled) {isEnabled = enabled;}
    bool getEnabled() {return isEnabled;}
    
    void setGain(float newGain)
    {
        if (gain < 0 || gain > 1)
            gain = .5;
        
        else
            gain = newGain;
        
    }
    
    float getGain()
    {
        return gain;
    }
    
    Playhead* playhead;
    
private:
    float gain = .1;
    bool shouldPlay = false;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioFormatManager formatManager;
    AudioTransportSource transportSource;
    
    bool isEnabled = true;
};
