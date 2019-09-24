/*
 ==============================================================================
 
 LevelMeter.cpp
 Created: 23 Sep 2019 9:39:29am
 Author:  Quin Scacheri
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "LevelMeter.h"
#include "quinUtils.h"

//==============================================================================
LevelMeter::LevelMeter(std::function<float()> updateFunc)
{
    setColour(meterColourId, Colours::green);
    setColour(backgroundColourId, Colours::white);
    setColour(outlineColourId, Colours::black);
    
    updateLevel = updateFunc;
    
    startTimerHz(5);
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (Graphics& g)
{
    Path p;
    int w = getWidth();
    int h = getHeight();
    bool isVertical;
    
    if (getHeight() >= getWidth())
        isVertical = true;
    else
        isVertical = false;
    
    switch(options.shape)
    {
        case LevelMeter::Shapes::rectangle:
            p.addRectangle(getLocalBounds());
            break;
        case LevelMeter::Shapes::roundedRectangle:
            p.addRoundedRectangle(getLocalBounds().toFloat(), getHeight() * options.cornerSizeRatio);
            break;
        default:
            p.addRectangle(getLocalBounds());
            break;
    }
    float border = h * options.borderThicknessRatio;
    
    g.setColour(findColour(ColourIds::backgroundColourId));
    g.fillPath(p);
    
    g.setColour(findColour(ColourIds::meterColourId));
    
    if (isVertical)
    {
        float levelHeight = qtils::map(currentLevel, 0.f, 1.f, 0, getHeight());
        g.fillRect(border, getHeight() - levelHeight, getWidth() - border, levelHeight);

    }
    else
    {
        float levelWidth = qtils::map(currentLevel, 0.f, 1.f, 0, getWidth());
        g.fillRect(border, border, levelWidth, getHeight() - border);
    }
        
    p.applyTransform(AffineTransform::scale(1.f - border / w, 1.f - border / h, w / 2, h / 2));
    g.setColour(findColour(ColourIds::outlineColourId));
    g.strokePath(p, PathStrokeType(h * options.borderThicknessRatio));
    
    
}

void LevelMeter::timerCallback()
{
    currentLevel = updateLevel();
    repaint();
}
