/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  7 Jan 2010 1:49:29 pm

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

#include "SearchComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace boost;
//[/MiscUserDefs]

//==============================================================================
SearchComponent::SearchComponent ()
    : _font(14.0f),
      searchButton (0),
      textEditor (0),
      searchlistTable (0)
{
    addAndMakeVisible (searchButton = new TextButton (T("search")));
    searchButton->setButtonText (T("Go"));
    searchButton->addButtonListener (this);

    addAndMakeVisible (textEditor = new TextEditor (T("new text editor")));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (String::empty);

    addAndMakeVisible (searchlistTable = new TableListBox (T("searchlist"), this));
    searchlistTable->setName (T("searchlist"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 400);

    //[Constructor] You can add your own custom stuff here..
	textEditor->setTooltip(T("Enter search text here"));
	textEditor->setSelectAllWhenFocused(true);
	textEditor->addListener(this);

	searchlistTable->setColour(ListBox::outlineColourId, Colours::grey);
	searchlistTable->setOutlineThickness(1);

	searchlistTable->getHeader()->addColumn(T("Track"), 1, 200, 30, -1, TableHeaderComponent::notSortable);
	searchlistTable->getHeader()->addColumn(T("Artist"), 2, 50, 30, -1, TableHeaderComponent::notSortable);
	searchlistTable->getHeader()->addColumn(T("Album"), 3, 50, 30, -1, TableHeaderComponent::notSortable);

    //[/Constructor]
}

SearchComponent::~SearchComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (searchButton);
    deleteAndZero (textEditor);
    deleteAndZero (searchlistTable);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SearchComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SearchComponent::resized()
{
    searchButton->setBounds (getWidth() - 48, 8, 39, 24);
    textEditor->setBounds (8, 8, getWidth() - 60, 24);
    searchlistTable->setBounds (8, 40, getWidth() - 16, getHeight() - 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SearchComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == searchButton)
    {
        //[UserButtonCode_searchButton] -- add your button handler code here..
		doSearch();
        //[/UserButtonCode_searchButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
int SearchComponent::getNumRows()
{
	return _searchResult.size();
	//return AudioPlayer::getInstance()->getPlaylist()->getCount();
}

void SearchComponent::paintRowBackground( Graphics& g, int rowNumber, int width, int height, bool rowIsSelected )
{
	if (rowIsSelected)
		g.fillAll (Colours::lightblue);
}

void SearchComponent::paintCell( Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected )
{
	g.setColour(Colours::black);
	g.setFont(_font);

	shared_ptr<SongInfo> songInfo = _searchResult.at(rowNumber);
	switch(columnId)
	{
	case 1:
		g.drawText(songInfo->getTitle(), 2, 0, width - 4, height, Justification::centredLeft, true);
		break;
	case 2:
		g.drawText(songInfo->getArtist(), 2, 0, width - 4, height, Justification::centredLeft, true);
		break;
	case 3:
		g.drawText(songInfo->getAlbum(), 2, 0, width - 4, height, Justification::centredLeft, true);
		break;
	}

	//g.setColour (Colours::black.withAlpha (0.2f));
	//g.fillRect (width - 1, 0, 1, height);
}


void SearchComponent::cellDoubleClicked( int rowNumber, int columnId, const MouseEvent& e )
{
	// Submit a new playlist
	// TODO: This plumbing should be done in lower layers
	shared_ptr<Playlist> playlist( new Playlist() );
	for(int i = rowNumber; i < _searchResult.size(); i++)
	{
		shared_ptr<SongInfo> songInfo = _searchResult.at(i);
		String url = ServiceManager::getInstance()->getClodder()->getSongUrl(songInfo->getSongID());
		shared_ptr<PlaylistEntry> entry( new PlaylistEntry(songInfo, url));
		playlist->add(entry);
	}

	// Set new playlist and ensure it is being played
	AudioPlayer* player = AudioPlayer::getInstance();
	player->setPlaylist(playlist);
	if(player->getPlayerStatus() != Player::Playing)
		player->startPlaying();
}

void SearchComponent::textEditorTextChanged( TextEditor & editor )
{

}

void SearchComponent::textEditorReturnKeyPressed( TextEditor & editor )
{
	doSearch();
}

void SearchComponent::textEditorEscapeKeyPressed( TextEditor & editor )
{

}

void SearchComponent::textEditorFocusLost( TextEditor & editor )
{

}

void SearchComponent::doSearch()
{
	String text = textEditor->getText().trim();
	if(text.length() == 0)
		return;

	String result = ServiceManager::getInstance()->getClodder()->searchRaw(text);
	
	ModelBuilder builder;
	_searchResult = builder.createFromSearchResult(result);

	searchlistTable->updateContent();
	searchlistTable->repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SearchComponent" componentName=""
                 parentClasses="public Component, public TableListBoxModel, public TextEditorListener"
                 constructorParams="" variableInitialisers="_font(14.0f)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330000013" fixedSize="0"
                 initialWidth="400" initialHeight="400">
  <BACKGROUND backgroundColour="ffffff"/>
  <TEXTBUTTON name="search" id="97800ff1382c8fb0" memberName="searchButton"
              virtualName="" explicitFocusOrder="0" pos="48R 8 39 24" buttonText="Go"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="6d52bd45f42a379" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="8 8 60M 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <GENERICCOMPONENT name="searchlist" id="b2c21b70104bb812" memberName="searchlistTable"
                    virtualName="" explicitFocusOrder="0" pos="8 40 16M 40M" class="TableListBox"
                    params="T(&quot;searchlist&quot;), this"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
