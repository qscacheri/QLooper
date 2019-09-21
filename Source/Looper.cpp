/*
 ==============================================================================
 
 Looper.cpp
 Created: 15 Sep 2019 10:52:43am
 Author:  Quin Scacheri
 
 ==============================================================================
 */

#include "Looper.h"

Looper::Looper() : playhead(), metronome(&playhead)
{
    playhead.setLength(playhead.beatsToSamples(4 * 16));
    loops.push_back(std::make_unique<Loop>());
    loops[0]->audioData.setSize(1, playhead.getLength());
    loops[0]->audioData.clear();
}

void Looper::prepareToPlay(int samplesExpected, double sampleRate)
{
    lastSampleRate = sampleRate;
    metronome.prepareToPlay(samplesExpected, sampleRate);

}

void Looper::getNextAudioBlock(AudioBuffer<float> &buffer)
{
    int numSamples = buffer.getNumSamples();
    
    playhead.updatePosition(numSamples);
    
    if (isRecording)
    {
        if (playhead.getPositionInSamples() + numSamples >= playhead.getLength())
        {
            int diff = playhead.getLength() - playhead.getPositionInSamples();
            recordBuffer.copyFrom(0, playhead.getPositionInSamples(), buffer, 0, 0, diff);
            recordBuffer.copyFrom(0, 0, buffer, 0, diff, numSamples - diff);
            
            
        }
        else
            recordBuffer.copyFrom(0, playhead.getPositionInSamples(), buffer, 0, 0, buffer.getNumSamples());
    }
    
    if (!inputMonitoringEnabled) {
        buffer.clear();
    }
    if (playhead.getPositionInSamples() + numSamples >= playhead.getLength())
    {
        int diff = playhead.getLength() - playhead.getPositionInSamples();
        buffer.addFrom(0, 0, loops[0]->audioData, 0, playhead.getPositionInSamples(), diff);
        buffer.addFrom(0, diff, loops[0]->audioData, 0, 0, numSamples - diff);
    }
    
    else
        buffer.addFrom(0, 0, loops[0]->audioData, 0, playhead.getPositionInSamples(), numSamples);
    
    if (getMetronomeEnabled()) {
        metronome.getNextBlock(AudioSourceChannelInfo(buffer));
    }


}

void Looper::startRecording(bool createNewLoop)
{
    recordBuffer.setSize(1, playhead.getLength());
    recordBuffer.clear();
    
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
    
    loops[0]->audioData.addFrom(0, 0, recordBuffer, 0, 0, playhead.getLength());
    
}

void Looper::setMetronomeEnabled(bool metronomeEnabled)
{
    metronome.setEnabled(metronomeEnabled);
}
bool Looper::getMetronomeEnabled()
{
    return metronome.getEnabled();
}

void Looper::setTempo(int bpm)
{
    playhead.setTempo(bpm);
}

int Looper::getTempo()
{
    return playhead.getTempo();
}

void Looper::clearLoop()
{
    
}
