/*
  ==============================================================================

    LevelMeter.h
    Created: 23 Sep 2019 9:39:29am
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/

class LevelMeter : public Component, public Timer
{
public:
    
    enum ColourIds : int
    {
        meterColourId = 1,
        backgroundColourId,
        outlineColourId
    };
    
    enum Shapes
    {
        rectangle = 1,
        roundedRectangle
    };
    
    struct Options
    {
        Shapes shape = Shapes::rectangle;
        float borderThicknessRatio = .05;
        float cornerSizeRatio = .1;
        
    };
    
    LevelMeter(std::function<float()> updateFunc);
    ~LevelMeter();

    void paint (Graphics&) override;
    
    void timerCallback() override;
    
    void setOptions(Options newOptions) { options = newOptions; }
    
    std::function<float()> updateLevel;

private:
    float currentLevel = 0;
    Options options;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
