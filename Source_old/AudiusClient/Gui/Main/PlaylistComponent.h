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

#ifndef __JUCER_HEADER_PLAYLISTCOMPONENT_PLAYLISTCOMPONENT_11B266D8__
#define __JUCER_HEADER_PLAYLISTCOMPONENT_PLAYLISTCOMPONENT_11B266D8__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"

class PlaylistModel;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public ActionListener
{
public:
    //==============================================================================
    PlaylistComponent ();
    ~PlaylistComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	virtual void actionListenerCallback(const String& message);
	virtual int getNumRows();
	virtual void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);
	virtual void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
	virtual void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& e);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Font _font;
	Font _boldfont;

    //[/UserVariables]

    //==============================================================================
    TableListBox* playlistTable;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    PlaylistComponent (const PlaylistComponent&);
    const PlaylistComponent& operator= (const PlaylistComponent&);
};


#endif   // __JUCER_HEADER_PLAYLISTCOMPONENT_PLAYLISTCOMPONENT_11B266D8__
