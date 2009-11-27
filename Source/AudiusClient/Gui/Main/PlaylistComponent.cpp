/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  27 Nov 2009 8:49:11 am

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

#include "PlaylistModel.h"
#include "../../../AudiusLib/AudiusLib.h"
//[/Headers]

#include "PlaylistComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PlaylistComponent::PlaylistComponent (MusicPlayer* player)
    : _player(player),
      _font(14.0f),
      playlistTable (0)
{
    addAndMakeVisible (playlistTable = new TableListBox (T("playlist"), this));
    playlistTable->setName (T("playlist"));

    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 400);

    //[Constructor] You can add your own custom stuff here..
	playlistTable->setColour(ListBox::outlineColourId, Colours::grey);
	playlistTable->setOutlineThickness(1);

	playlistTable->getHeader()->addColumn(T("Test"), 1, 100, 30, -1, TableHeaderComponent::notSortable);

    //[/Constructor]
}

PlaylistComponent::~PlaylistComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (playlistTable);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PlaylistComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PlaylistComponent::resized()
{
    playlistTable->setBounds (8, 8, getWidth() - 16, getHeight() - 55);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
int PlaylistComponent::getNumRows()
{
	return _player->getPlaylistCount();
}

void PlaylistComponent::paintRowBackground( Graphics& g, int rowNumber, int width, int height, bool rowIsSelected )
{
	g.fillAll(Colours::red);
}

void PlaylistComponent::paintCell( Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected )
{
	g.setColour(Colours::black);
	g.setFont(_font);

	//const XmlElement* rowElement = dataList->getChildElement (rowNumber);

	//if (rowElement != 0)
	//{
	//	const String text (rowElement->getStringAttribute (getAttributeNameForColumnId (columnId)));

	//	g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
	//}

	//g.setColour (Colours::black.withAlpha (0.2f));
	//g.fillRect (width - 1, 0, 1, height);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PlaylistComponent" componentName=""
                 parentClasses="public Component, public TableListBoxModel" constructorParams="MusicPlayer* player"
                 variableInitialisers="_player(player),&#10;_font(14.0f)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330000013" fixedSize="0"
                 initialWidth="400" initialHeight="400">
  <BACKGROUND backgroundColour="ffffff"/>
  <GENERICCOMPONENT name="playlist" id="13011219828ab88e" memberName="playlistTable"
                    virtualName="" explicitFocusOrder="0" pos="8 8 16M 55M" class="TableListBox"
                    params="T(&quot;playlist&quot;), this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
