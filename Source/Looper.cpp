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
    for (int i = 0; i < 3; i++)
    {
        loops[i].reset(new Loop());
        loops[i]->audioData.setSize(1, playhead.getLength());
        loops[i]->audioData.clear();
    }
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
    
    for (int i = 0; i < 3; i++)
    {
        loops[i]->getNextAudioBlock(buffer, playhead);
    }

    
    if (getMetronomeEnabled()) {
        metronome.getNextBlock(AudioSourceChannelInfo(buffer));
    }


}

void Looper::startRecording(int which)
{
    recordBuffer.setSize(1, playhead.getLength());
    recordBuffer.clear();
    
    isRecording = true;
    DBG("STARTED RECORDING");
}

void Looper::stopRecording(int which)
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
    
    loops[which]->audioData.addFrom(0, 0, recordBuffer, 0, 0, playhead.getLength());
    
}

void Looper::toggleRecording(int which)
{
    if (isRecording)
        stopRecording(which);
    else
        startRecording(which);
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
    for (int i = 0; i < 3; i++){
        loops[i]->audioData.setSize(1, playhead.getLength());
    }
}

int Looper::getTempo()
{
    return playhead.getTempo();
}

void Looper::clearLoop(int which)
{
    if (isRecording)
        stopRecording(which);
    
    loops[which]->audioData.clear();
}

void Looper::setLoopGain(int which, float newGain)
{
    loops[which]->setGain(newGain);
}

float Looper::getLoopLevel(int which)
{
    return loops[which]->getLevel();
}

void Looper::setTempoLockEnabled(bool enabled)
{
    tempoLockEnabled = enabled;
}

bool Looper::getTempoLockEnabled()
{
    return tempoLockEnabled;
}

void Looper::toggleTempoLock()
{
    setTempoLockEnabled(!tempoLockEnabled);
}

