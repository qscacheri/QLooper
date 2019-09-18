/*
  ==============================================================================

    TwoPartSVGKnob.cpp
    Created: 17 Sep 2019 6:52:22pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TwoPartSVGKnob.h"

//==============================================================================
TwoPartSVGKnob::TwoPartSVGKnob(Image base, Image tick) : baseImage(base), tickImage(tick)
{
    
}

TwoPartSVGKnob::~TwoPartSVGKnob()
{
    
}

void TwoPartSVGKnob::paint(Graphics &g)
{
    g.drawRect(getLocalBounds(), 5.f);
    g.drawImageWithin(baseImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::centred);
    g.addTransform(AffineTransform::rotation(float_Pi / 2, getWidth() / 2, getHeight() / 2));
    g.drawImageWithin(tickImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::centred);
    Rectangle<int> rect(0, 0, tickImage.getWidth(), tickImage.getHeight());
    g.drawRect(rect, 2.f);
}
