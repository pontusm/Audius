/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  9 Jul 2009 4:37:37 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_LOGINCOMPONENT_LOGINCOMPONENT_3B32B903__
#define __JUCER_HEADER_LOGINCOMPONENT_LOGINCOMPONENT_3B32B903__

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
class LoginComponent  : public Component,
                        public ButtonListener
{
public:
    //==============================================================================
    LoginComponent ();
    ~LoginComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	const String getLogin();
	const String getPassword();
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
    TextEditor* textLogin;
    Label* label;
    Label* label2;
    TextEditor* textPassword;
    ToggleButton* toggleRemember;
    TextButton* buttonLogin;
    TextButton* buttonCancel;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    LoginComponent (const LoginComponent&);
    const LoginComponent& operator= (const LoginComponent&);
};


#endif   // __JUCER_HEADER_LOGINCOMPONENT_LOGINCOMPONENT_3B32B903__
