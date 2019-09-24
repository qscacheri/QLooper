/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "QLooperMenuBar.h"
//==============================================================================
class LooperApplication  : public JUCEApplication
{
public:
    //==============================================================================
    LooperApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow.reset (new MainWindow (getApplicationName()));
        
        

        ApplicationCommandInfo info(50);
        info.setInfo("Preferences", "Preferences", "Utility", 0);
        commandManager.registerCommand(info);
        MainComponent* target = static_cast<MainComponent*>(mainWindow->getContentComponent());
        target->keyMapper.reset(new KeyMappingEditorComponent(*commandManager.getKeyMappings(), true));
        
        commandManager.registerAllCommandsForTarget(target);
        commandManager.getKeyMappings()->resetToDefaultMappings();
        commandManager.getKeyMappings()->addKeyPress(QLooperMenuBarModel::ids::AudioPreferencesId, KeyPress(44, ModifierKeys::commandModifier, ','));
        target->addKeyListener (commandManager.getKeyMappings());

        model.reset(new QLooperMenuBarModel(&commandManager));
        PopupMenu appleMenu;
        appleMenu.addCommandItem(&commandManager, QLooperMenuBarModel::ids::AudioPreferencesId, "Preferences");
        MenuBarModel::setMacMainMenu(model.get(), &appleMenu);
        model->setApplicationCommandManagerToWatch(&commandManager);
        commandManager.setFirstCommandTarget(target);
        commandManager.invokeDirectly(QLooperMenuBarModel::ids::AudioPreferencesId, true);
        
        
        
        
        
        
    }

    void shutdown() override
    {
        
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    
    class MainWindow : public DocumentWindow
    {
    public:
        MainWindow (String name) : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };
    
    ApplicationCommandManager commandManager;

private:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<QLooperMenuBarModel> model;

};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (LooperApplication)
