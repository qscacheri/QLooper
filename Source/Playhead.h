/*
  ==============================================================================

    Playhead.h
    Created: 15 Sep 2019 11:20:10am
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Playhead
{
public:
    Playhead();
    Playhead(int sampleRate);
    ~Playhead();
    
    void updatePosition(int numSamples);
   
    int getPositionInSamples();

    int getLength() { return lengthInSamples; }
    void setLength(int numSamples);
    
    int beatsToSamples(int sixteenthNotes);
    
private:
    bool isPlaying = false;
    int posInSamples = 0;
    int lengthInSamples = 100;
    int bpm = 120;
    int lastSampleRate;
    
};

