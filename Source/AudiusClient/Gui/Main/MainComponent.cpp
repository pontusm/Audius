/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  22 Nov 2009 12:52:22 pm

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

#include "PlayerComponent.h"
#include "PlaylistComponent.h"
#include "../../../AudiusLib/AudiusLib.h"
//[/Headers]

#include "MainComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent (MusicPlayer* musicplayer, ApplicationCommandManager* commandManager)
    : _player(musicplayer),
      playerComp (0),
      playlistComp (0)
{
    addAndMakeVisible (playerComp = new PlayerComponent (musicplayer, commandManager));
    playerComp->setName (T("player"));

    addAndMakeVisible (playlistComp = new PlaylistComponent (musicplayer));
    playlistComp->setName (T("playlist"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (playerComp);
    deleteAndZero (playlistComp);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
	GradientBrush gradient_1 (Colours::white,
		208.0f, getHeight() - 140.0f,
		Colour (0xffbee6c8),
		240.0f, getHeight() - 20.0f,
		false);
	g.setBrush (&gradient_1);
	g.fillRect (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[/UserPaint]
}

void MainComponent::resized()
{
    playerComp->setBounds (0, getHeight() - 130, proportionOfWidth (1.0000f), 130);
    playlistComp->setBounds (0, 0, proportionOfWidth (1.0000f), getHeight() - 130);
    //[UserResized] Add your own custom resize handling here..
	//playerComp->setBounds (0, getHeight() - playerComp->getHeight(), proportionOfWidth (1.0000f), playerComp->getHeight());
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component" constructorParams="MusicPlayer* musicplayer, ApplicationCommandManager* commandManager"
                 variableInitialisers="_player(musicplayer)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="0 0 100% 100%" fill="linear: 208 32, 240 280, 0=ffffffff, 1=ffbee6c8"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="player" id="8288116db28ad9c9" memberName="playerComp" virtualName=""
                    explicitFocusOrder="0" pos="0 0Rr 100% 130" class="PlayerComponent"
                    params="musicplayer, commandManager"/>
  <GENERICCOMPONENT name="playlist" id="ae88366f48b8de1" memberName="playlistComp"
                    virtualName="" explicitFocusOrder="0" pos="0 0 100% 130M" class="PlaylistComponent"
                    params="musicplayer"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
