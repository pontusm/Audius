/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  27 Oct 2009 9:28:11 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_PLAYLISTCOMPONENT_PLAYLISTCOMPONENT_8DD9BD72__
#define __JUCER_HEADER_PLAYLISTCOMPONENT_PLAYLISTCOMPONENT_8DD9BD72__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"

class MusicPlayer;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PlaylistComponent  : public Component
{
public:
    //==============================================================================
    PlaylistComponent (MusicPlayer* player);
    ~PlaylistComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	MusicPlayer*	_player;
    //[/UserVariables]

    //==============================================================================
    TableListBox* playlistTable;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    PlaylistComponent (const PlaylistComponent&);
    const PlaylistComponent& operator= (const PlaylistComponent&);
};


#endif   // __JUCER_HEADER_PLAYLISTCOMPONENT_PLAYLISTCOMPONENT_8DD9BD72__
