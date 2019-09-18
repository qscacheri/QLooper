/*
  ==============================================================================

    TwoPartSVGKnob.h
    Created: 17 Sep 2019 6:52:22pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class TwoPartSVGKnob : public Slider
{
public:
    TwoPartSVGKnob(Image base, Image tick);
    ~TwoPartSVGKnob();

    void paint(Graphics &g) override;
    
private:
    Image baseImage;
    Image tickImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoPartSVGKnob)
};
