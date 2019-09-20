/*
  ==============================================================================

    QLooperLookAndFeel.h
    Created: 19 Sep 2019 6:23:49pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class QLooperLookAndFeel : public LookAndFeel_V4
{
public:
    QLooperLookAndFeel(){}
    
    void drawComboBox (Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ComboBox &) override
    {
        g.fillAll(Colours::white);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QLooperLookAndFeel)
};
