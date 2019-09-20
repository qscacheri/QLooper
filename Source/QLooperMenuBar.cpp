/*
  ==============================================================================

    QLooperMenuBar.cpp
    Created: 19 Sep 2019 4:19:11pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "QLooperMenuBar.h"
#include "MainComponent.h"

QLooperMenuBarModel::QLooperMenuBarModel(MainComponent* mc)
{
    owner = mc;
}

StringArray QLooperMenuBarModel::getMenuBarNames()
{
    StringArray names;
    names.add("Preferences");
    
    return names;
}

PopupMenu QLooperMenuBarModel::getMenuForIndex (int topLevelMenuIndex, const String &menuName)
{
    PopupMenu menu;
    
    if (menuName.equalsIgnoreCase("Preferences"))
    {
        menu.addItem(PopupMenu::Item("Audio Setup"));
        return menu;
    }
}

void QLooperMenuBarModel::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    owner->audioWindow->setVisible(true);
}

void QLooperMenuBarModel::menuBarActivated (bool isActive)
{
    
}
