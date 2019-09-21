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

class Looper
{
public:
    Looper();
    
    void prepareToPlay(int samplesExpected, double sampleRate);
    
    void getNextAudioBlock(AudioBuffer<float> &buffer);
        
    void startRecording(bool createNewLoop = false);
    
    void stopRecording();
    
    void setInputMonitoring(bool inputMonitoring) { inputMonitoringEnabled = inputMonitoring; }
    
    bool getInputMonitoring() {return inputMonitoringEnabled;}
    
    void setMetronomeEnabled(bool enabled);
    bool getMetronomeEnabled();
    
    void setTempo(int bpm);
    int getTempo();
    
    void clearLoop();
    
    bool isRecording = false;
    
    

private:
    Playhead playhead;
    
    Metronome metronome;
    
    std::vector<std::unique_ptr<Loop>> loops;
    double lastSampleRate = 44100;
    
    AudioBuffer<float> recordBuffer;
    
    bool inputMonitoringEnabled = false;
};
