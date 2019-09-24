/*
  ==============================================================================

    Looper.h
    Created: 15 Sep 2019 10:52:43am
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Loop.h"
#include "Playhead.h"
#include "Metronome.h"
#include <array>

class Looper
{
public:
    Looper();
    
    void prepareToPlay(int samplesExpected, double sampleRate);
    
    void getNextAudioBlock(AudioBuffer<float> &buffer);
        
    void startRecording(int which);
    void stopRecording(int which);
    void toggleRecording(int which);
    
    void setInputMonitoring(bool inputMonitoring) { inputMonitoringEnabled = inputMonitoring; }
    
    bool getInputMonitoring() {return inputMonitoringEnabled;}
    
    void setMetronomeEnabled(bool enabled);
    bool getMetronomeEnabled();
    
    void setTempo(int bpm);
    int getTempo();
    
    void clearLoop(int which);
    
    bool isRecording = false;
    
    bool getActive(int which) { return loops[which]->getActive(); }
    void setActive(int which, bool isActive) {loops[which]->setActive(isActive); }
    
    void setLoopGain(int which, float newGain);
    float getLoopLevel(int which);
    
    void setTempoLockEnabled(bool should);
    bool getTempoLockEnabled();
    void toggleTempoLock();

private:
    Playhead playhead;
    
    Metronome metronome;
    
    std::array<std::unique_ptr<Loop>, 3> loops;
    double lastSampleRate = 44100;
    
    AudioBuffer<float> recordBuffer;
    
    bool inputMonitoringEnabled = false;
    
    bool tempoLockEnabled = true;
};
