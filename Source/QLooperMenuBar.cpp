/*
  ==============================================================================

    QLooperMenuBar.cpp
    Created: 19 Sep 2019 4:19:11pm
    Author:  Quin Scacheri

  ==============================================================================
*/

#include "QLooperMenuBar.h"
#include "MainComponent.h"

QLooperMenuBarModel::QLooperMenuBarModel(ApplicationCommandManager* manager)
{
    commandManager = manager;
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
    menu.addCommandItem(commandManager, 10, "slkdfjs");

    if (menuName.equalsIgnoreCase("Preferences"))
    {
        menu.addItem(PopupMenu::Item("Audio Setup"));
        return menu;
    }
    return PopupMenu();
}

void QLooperMenuBarModel::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    if (menuItemID == ids::AudioPreferencesId)
    {
//        owner->audioWindow->setVisible(true);
//        owner->audioWindow->toFront(true);
    }
}

void QLooperMenuBarModel::menuBarActivated (bool isActive)
{
    
}
