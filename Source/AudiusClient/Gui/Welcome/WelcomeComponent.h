/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  25 Aug 2009 9:38:27 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_WELCOMECOMPONENT_WELCOMECOMPONENT_595DE1AC__
#define __JUCER_HEADER_WELCOMECOMPONENT_WELCOMECOMPONENT_595DE1AC__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WelcomeComponent  : public Component,
                          public ButtonListener
{
public:
    //==============================================================================
    WelcomeComponent ();
    ~WelcomeComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Label* label;
    Label* label2;
    ToggleButton* toggleYes;
    ToggleButton* toggleRemind;
    ToggleButton* toggleNo;
    TextButton* textButton;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    WelcomeComponent (const WelcomeComponent&);
    const WelcomeComponent& operator= (const WelcomeComponent&);
};


#endif   // __JUCER_HEADER_WELCOMECOMPONENT_WELCOMECOMPONENT_595DE1AC__
