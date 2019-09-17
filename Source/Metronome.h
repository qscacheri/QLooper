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
    Metronome()
    {
        formatManager.registerBasicFormats();
        std::unique_ptr<MemoryInputStream> inputStream(new MemoryInputStream(BinaryData::click_wav, BinaryData::click_wavSize, false));
        
        auto* reader = formatManager.createReaderFor (inputStream.get());
        if (reader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
            transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            readerSource.reset (newSource.release());
            
        }
        
//        readerSource->setLooping(true);
        inputStream.release();
    }
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        readerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
        readerSource->setLooping(true);
    }

    void getNextBlock(const AudioSourceChannelInfo& bufferToFill)
    {
        readerSource->getNextAudioBlock(bufferToFill);
//        for (int i = 0; i < 16; i++)
//        {
//            if (playhead->getPositionInSamples() + bufferToFill.buffer->getNumSamples() > playhead->beatsToSamples(i * 4) && playhead->getPositionInSamples() <= playhead->beatsToSamples(i * 4))
//                shouldPlay = true;
//        }
//
//
//        if (readerSource->getNextReadPosition() > readerSource->getTotalLength())
//        {
//            shouldPlay = false;
//            readerSource->setNextReadPosition(0);
//        }
//
//        if (shouldPlay)
//        {
//            readerSource->getNextAudioBlock(bufferToFill);
//        }

    }
    
    Playhead* playhead;
    
private:
    bool shouldPlay = false;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioFormatManager formatManager;
    AudioTransportSource transportSource;
};
