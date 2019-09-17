/*
 ==============================================================================
 
 Looper.cpp
 Created: 15 Sep 2019 10:52:43am
 Author:  Quin Scacheri
 
 ==============================================================================
 */

#include "Looper.h"

Looper::Looper()
{
    
}

void Looper::prepareToPlay(int samplesExpected, double sampleRate)
{
    lastSampleRate = sampleRate;
}


void Looper::getNextAudioBlock(AudioBuffer<float> &buffer)
{
    int numSamples = buffer.getNumSamples();
    if (isRecording)
    {
        
        if (playhead->getPositionInSamples() + numSamples >= playhead->getLength())
        {
            int diff = playhead->getLength() - playhead->getPositionInSamples();
            loops[0]->audioData.copyFrom(0, playhead->getPositionInSamples(), buffer, 0, 0, diff);
            loops[0]->audioData.copyFrom(0, 0, buffer, 0, diff, numSamples - diff);
            
            
        }
        else
            loops[0]->audioData.copyFrom(0, playhead->getPositionInSamples(), buffer, 0, 0, buffer.getNumSamples());
    }
    
    else if (loops.size() > 0)
    {
        if (playhead->getPositionInSamples() + numSamples >= playhead->getLength())
        {
            int diff = playhead->getLength() - playhead->getPositionInSamples();
            buffer.addFrom(0, 0, loops[0]->audioData, 0, playhead->getPositionInSamples(), diff);
            buffer.addFrom(0, diff, loops[0]->audioData, 0, 0, numSamples - diff);
        }

        else
            buffer.addFrom(0, 0, loops[0]->audioData, 0, playhead->getPositionInSamples(), numSamples);
    }
}

void Looper::startRecording(bool createNewLoop)
{
    int loopSamples = playhead->beatsToSamples(16 * 4);
    loops.push_back(std::make_unique<Loop>());
    loops[0]->audioData.setSize(1, loopSamples);
    loops[0]->audioData.clear();

    isRecording = true;
    DBG("STARTED RECORDING");
}

void Looper::stopRecording()
{
    File file("~/Desktop/out.wav");
    std::unique_ptr<FileOutputStream> outputStream(file.createOutputStream());
    WavAudioFormat wav;
    if (outputStream != nullptr)
    {
        std::shared_ptr<AudioFormatWriter> writer (wav.createWriterFor (outputStream.get(), lastSampleRate, 1, 24, "", 0));
        if (writer != nullptr)
        {

//            writer->.writeFromAudioSampleBuffer (loops[0]->audioData, 0, loops[0]->audioData.getNumSamples());

        }
    }
    isRecording = false;
    outputStream.release();

    
    DBG("STOPPED RECORDING");

}
