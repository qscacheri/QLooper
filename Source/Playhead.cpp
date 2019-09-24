/*
  ==============================================================================

    Playhead.cpp
    Created: 15 Sep 2019 11:20:10am
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "Playhead.h"

Playhead::Playhead() : lastSampleRate(48000)
{
    
}

Playhead::Playhead(int sampleRate)
{
    lastSampleRate = sampleRate;
    lengthInSamples = beatsToSamples(16 * 4);
}

Playhead::~Playhead()
{
    
}

void Playhead::updatePosition(int numSamples)
{
    posInSamples += numSamples;
    posInSamples %= lengthInSamples;
}

int Playhead::getPositionInSamples()
{
    return posInSamples;
}


void Playhead::setLength(int numSamples)
{
    if (numSamples > 0)
        lengthInSamples = numSamples;
}


int Playhead::beatsToSamples(int sixteenthNotes)
{
    int samples = ((int)((60.0 / bpm * lastSampleRate) / 4 * sixteenthNotes));
    return samples;
}

void Playhead::setTempo(int newTempo)
{
    bpm = newTempo;
}
int Playhead::getTempo()
{
    return bpm;
}
