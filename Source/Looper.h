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

class Looper
{
public:
    Looper();
    
    void prepareToPlay(int samplesExpected, double sampleRate);
    
    void getNextAudioBlock(AudioBuffer<float> &buffer);
        
    void startRecording(bool createNewLoop = false);
    
    void stopRecording();
    
    Playhead* playhead;
    
    bool isRecording = false;

private:
    std::vector<std::unique_ptr<Loop>> loops;
    double lastSampleRate = 44100;
};
