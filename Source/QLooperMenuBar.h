/*
  ==============================================================================

    QLooperMenuBar.h
    Created: 18 Sep 2019 8:18:36pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent;

class QLooperMenuBarModel : public MenuBarModel
{
public:
    QLooperMenuBarModel (MainComponent* mainComponent);
    
    virtual StringArray getMenuBarNames() override;
    
    virtual PopupMenu getMenuForIndex (int topLevelMenuIndex, const String &menuName) override;
    
    virtual void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    
    virtual void menuBarActivated (bool isActive) override;
    
    MainComponent* owner;
};
