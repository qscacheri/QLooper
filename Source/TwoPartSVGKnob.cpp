/*
  ==============================================================================

    TwoPartSVGKnob.cpp
    Created: 17 Sep 2019 6:52:22pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TwoPartSVGKnob.h"
#include "quinUtils.h"
//==============================================================================
TwoPartSVGKnob::TwoPartSVGKnob(Image base, Image tick) : baseImage(base), tickImage(tick)
{
    setSliderStyle(SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(NoTextBox, true, 0, 0);
    
}

TwoPartSVGKnob::~TwoPartSVGKnob()
{
    
}

void TwoPartSVGKnob::paint(Graphics &g)
{
    
    g.drawImageWithin(baseImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::centred);
    
    float angle = qtils::map(getValue(), getMinimum(), getMaximum(), -2.42, 2.42);
    
    g.addTransform(AffineTransform::rotation(angle, getWidth() / 2, getHeight() / 2));
    g.drawImageWithin(tickImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);
    
}
