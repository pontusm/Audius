/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Jan 2010 3:08:25 pm

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

#include "../../../AudiusLib/AudiusLib.h"
//[/Headers]

#include "PlaylistComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PlaylistComponent::PlaylistComponent ()
    : _font(14.0f),
	_boldfont(14.0f, Font::bold),
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

	playlistTable->getHeader()->addColumn(T("Track"), 1, 250, 30, -1, TableHeaderComponent::notSortable);
	playlistTable->getHeader()->addColumn(T("Artist"), 2, 150, 30, -1, TableHeaderComponent::notSortable);
	playlistTable->getHeader()->addColumn(T("Length"), 3, 50, 30, -1, TableHeaderComponent::notSortable);
	//playlistTable->getHeader()->setStretchToFitActive(true);

	AudioPlayer::getInstance()->addActionListener(this);
    //[/Constructor]
}

PlaylistComponent::~PlaylistComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	AudioPlayer::getInstance()->removeActionListener(this);
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
    playlistTable->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
int PlaylistComponent::getNumRows()
{
	return AudioPlayer::getInstance()->getPlaylist()->getCount();
}

void PlaylistComponent::paintRowBackground( Graphics& g, int rowNumber, int width, int height, bool rowIsSelected )
{
	if (rowIsSelected)
		g.fillAll (Colours::lightblue);
}

void PlaylistComponent::paintCell( Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected )
{
	AudioPlayer* player = AudioPlayer::getInstance();

	std::shared_ptr<Playlist> playlist = player->getPlaylist();
	std::shared_ptr<PlaylistEntry> playlistEntry = playlist->getEntry(rowNumber);
	if(playlistEntry)
	{
		g.setColour(Colours::black);

		std::shared_ptr<SongInfo> songInfo = playlistEntry->getSongInfo();
		if(songInfo == player->getCurrentSong())
			g.setFont(_boldfont);
		else
			g.setFont(_font);

		switch(columnId)
		{
		case 1:
			g.drawText(songInfo->getTitle(), 2, 0, width - 4, height, Justification::centredLeft, true);
			break;
		case 2:
			g.drawText(songInfo->getArtist(), 2, 0, width - 4, height, Justification::centredLeft, true);
			break;
		case 3:
			int seconds = songInfo->getLengthSeconds();
			if(seconds > 0)
			{
				const String s = String::formatted(T("%d:%02d"), (seconds / 60), (seconds % 60) );
				g.drawText(s, 2, 0, width - 4, height, Justification::centredRight, true);
			}
			break;
		}
	}

	//g.setColour (Colours::black.withAlpha (0.2f));
	//g.fillRect (width - 1, 0, 1, height);
}

void PlaylistComponent::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& e)
{
	AudioPlayer::getInstance()->setCurrentPlaylistPosition(rowNumber, true);
}

void PlaylistComponent::actionListenerCallback(const String& message)
{
	if(message == PlayerNotifications::playlistChanged || message == PlayerNotifications::songInfoChanged)
	{
		playlistTable->updateContent();
		playlistTable->repaint();
	}
	else if(message == PlayerNotifications::newSong)
	{
		playlistTable->repaint();
	}
	else if(message == PlayerNotifications::serverPlaylistUpdated)
	{
		// Need to hide search and display playlist
		setVisible(true);
	}
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PlaylistComponent" componentName=""
                 parentClasses="public Component, public TableListBoxModel, public ActionListener"
				 constructorParams="" variableInitialisers="_font(14.0f), _boldfont(14.0f, Font::bold)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330000013" fixedSize="0"
                 initialWidth="400" initialHeight="400">
  <BACKGROUND backgroundColour="ffffff"/>
  <GENERICCOMPONENT name="playlist" id="13011219828ab88e" memberName="playlistTable"
                    virtualName="" explicitFocusOrder="0" pos="8 8 16M 16M" class="TableListBox"
                    params="T(&quot;playlist&quot;), this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
