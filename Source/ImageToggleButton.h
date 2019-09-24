/*
  ==============================================================================

    ImageToggleButton.h
    Created: 21 Sep 2019 1:41:08pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ImageToggleButton : public ToggleButton
{
public:
    enum stateIds : int
    {
        normalUp = 1,
        normalDown,
        onUp,
        onDown
    };
    
    ImageToggleButton()
    {
        setOpaque(false);
    }

    ~ImageToggleButton()
    {
    }

    void paintButton (Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        Image imageToDraw;
        if (getToggleState())
        {
            if (shouldDrawButtonAsDown)
                imageToDraw = images[stateIds::onDown];
            else
                imageToDraw = images[stateIds::onUp];

        }
        else
        {
            if (shouldDrawButtonAsDown)
                imageToDraw = images[stateIds::normalDown];
            else
                imageToDraw = images[stateIds::normalUp];

        }
        
        g.drawImageWithin(imageToDraw, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit, false);
        
//        g.drawRect(getLocalBounds(), 1.f);
    }
    
    void setImages(Image normalUp, Image normalDown, Image onUp, Image onDown)
    {
        images[stateIds::normalUp] = normalUp;
        images[stateIds::normalDown] = normalDown;
        images[stateIds::onUp] = onUp;
        images[stateIds::onDown] = onDown;

    }

private:
    std::map<int, Image> images;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageToggleButton)
};
