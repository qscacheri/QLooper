/*
 ==============================================================================
 
 Loop.h
 Created: 15 Sep 2019 10:52:26am
 Author:  Quin Scacheri
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Playhead.h"
#define MAX_LOOP_SIZE 14400000

class Loop
{
public:
    
    Loop() : audioData()
    {
        
    }
    
    void getNextAudioBlock(AudioBuffer<float> &buffer, Playhead &playhead)
    {
        if (!isActive)
            return;
        
        int numSamples = buffer.getNumSamples();
        
        AudioBuffer<float> currentBuffer(buffer.getNumChannels(), numSamples);
        currentBuffer.clear();
        
        if (playhead.getPositionInSamples() + numSamples >= playhead.getLength())
        {
            int diff = playhead.getLength() - playhead.getPositionInSamples();
            currentBuffer.addFrom(0, 0, audioData, 0, playhead.getPositionInSamples(), diff);
            currentBuffer.addFrom(0, diff, audioData, 0, 0, numSamples - diff);
        }
        
        else
        {
            currentBuffer.addFrom(0, 0, audioData, 0, playhead.getPositionInSamples(), numSamples);
            
        }
        
        for (int i = 0; i < buffer.getNumChannels(); i++){
            currentBuffer.applyGain(gain);
            buffer.addFrom(i, 0, currentBuffer, 0, 0, numSamples);
        }
        
        currentLevel = audioData.getRMSLevel(0, playhead.getPositionInSamples(), numSamples);
    }
    
    float getLevel() { return currentLevel; }
    
    void doubleLoop();
    
    bool getActive() { return isActive; }
    void setActive(bool active) { isActive = active; }
    
    void setGain(float newGain)
    {
        if (gain > 1 || gain < 0)
            gain = .5;
        else
            gain = newGain;
    }
    
    AudioBuffer<float> audioData;
    
private:
    
    float currentLevel = 0;
    
    bool isActive = true;
    
    float gain = 1;
};
