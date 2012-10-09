/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Jan 2010 2:58:41 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Precompiled.h"
//[/Headers]

#include "WelcomeComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
WelcomeComponent::WelcomeComponent ()
    : label (0),
      label2 (0),
      toggleYes (0),
      toggleRemind (0),
      toggleNo (0),
      textButton (0)
{
    addAndMakeVisible (label = new Label (String::empty,
                                          "Would you like to add your music to Clodder?"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label (String::empty,
                                           "Welcome to Audius\nmusic player for Clodder"));
    label2->setFont (Font (25.1000f, Font::bold));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (toggleYes = new ToggleButton ("Yes"));
    toggleYes->setButtonText ("Yes (recommended)");
    toggleYes->setRadioGroupId (1);
	toggleYes->addListener(this);

    addAndMakeVisible (toggleRemind = new ToggleButton ("Remind"));
    toggleRemind->setButtonText ("Remind me later");
    toggleRemind->setRadioGroupId (1);
    toggleRemind->addListener(this);

    addAndMakeVisible (toggleNo = new ToggleButton ("No"));
    toggleNo->setRadioGroupId (1);
    toggleNo->addListener(this);

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText ("Next ->");
    textButton->addListener(this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (380, 280);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WelcomeComponent::~WelcomeComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (toggleYes);
    deleteAndZero (toggleRemind);
    deleteAndZero (toggleNo);
    deleteAndZero (textButton);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WelcomeComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    ColourGradient gradient_1 (Colours::white,
                              152.0f, 24.0f,
                              Colour (0xffbee6c8),
                              240.0f, 280.0f,
                              false);
    g.setGradientFill(gradient_1);
    g.fillRect (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));

    g.setColour (Colour (0xff2aa581));
    g.fillEllipse ((float) (-28), (float) (-28), 100.0f, 100.0f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WelcomeComponent::resized()
{
    label->setBounds (8, 88, 368, 24);
    label2->setBounds (8, 8, 368, 64);
    toggleYes->setBounds (120, 128, 150, 24);
    toggleRemind->setBounds (120, 152, 150, 24);
    toggleNo->setBounds (120, 176, 150, 24);
    textButton->setBounds (216, 232, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void WelcomeComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleYes)
    {
        //[UserButtonCode_toggleYes] -- add your button handler code here..
        //[/UserButtonCode_toggleYes]
    }
    else if (buttonThatWasClicked == toggleRemind)
    {
        //[UserButtonCode_toggleRemind] -- add your button handler code here..
        //[/UserButtonCode_toggleRemind]
    }
    else if (buttonThatWasClicked == toggleNo)
    {
        //[UserButtonCode_toggleNo] -- add your button handler code here..
        //[/UserButtonCode_toggleNo]
    }
    else if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WelcomeComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="380" initialHeight="280">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="0 0 100% 100%" fill="linear: 152 24, 240 280, 0=ffffffff, 1=ffbee6c8"
          hasStroke="0"/>
    <ELLIPSE pos="-28 -28 100 100" fill="solid: ff2aa581" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="" id="78e526e81f48e65" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 88 368 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Would you like to add your music to Clodder?"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="4610de6778e61a4a" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 8 368 64" edTextCol="ff000000"
         edBkgCol="0" labelText="Welcome to Audius&#10;music player for Clodder"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="25.1" bold="1" italic="0" justification="36"/>
  <TOGGLEBUTTON name="Yes" id="86f3025b7c502d7e" memberName="toggleYes" virtualName=""
                explicitFocusOrder="0" pos="120 128 150 24" buttonText="Yes (recommended)"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="Remind" id="ccd6cb374536e552" memberName="toggleRemind"
                virtualName="" explicitFocusOrder="0" pos="120 152 150 24" buttonText="Remind me later"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="No" id="e1b119a17130d8a0" memberName="toggleNo" virtualName=""
                explicitFocusOrder="0" pos="120 176 150 24" buttonText="No" connectedEdges="0"
                needsCallback="1" radioGroupId="1" state="0"/>
  <TEXTBUTTON name="new button" id="b386e30f8cf1410f" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="216 232 150 24" buttonText="Next -&gt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
