/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  7 Jan 2010 12:53:02 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_2BB1072D__
#define __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_2BB1072D__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"

#include "../../Resources/ResourceFiles.h"

class PlayerComponent;
class PlaylistComponent;
class SearchComponent;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainComponent  : public Component,
                       public ButtonListener
{
public:
    //==============================================================================
    MainComponent (ApplicationCommandManager* commandManager);
    ~MainComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void buttonClicked(Button* button);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	class MainToolbarFactory : public ToolbarItemFactory
	{
	public:
		MainToolbarFactory() {}
		~MainToolbarFactory() {}

		enum ToolbarItemIds
		{
			search	= 1
		};

		void getAllToolbarItemIds(Array<int>& ids)
		{
			ids.add(search);
		}

		void getDefaultItemSet(Array<int>& ids)
		{
			ids.add(search);
		}

		ToolbarItemComponent* createItem(int itemId)
		{
			switch(itemId)
			{
			case search:
				searchButton = new ToolbarButton(itemId, "Search", Drawable::createFromImageData(ResourceFiles::view_png, ResourceFiles::view_pngSize), 0);
				return searchButton;
			}
			return NULL;
		}

		ToolbarButton* searchButton;
	};

	MainToolbarFactory toolbarFactory;

	SearchComponent* searchComp;
    //[/UserVariables]

    //==============================================================================
    PlayerComponent* playerComp;
    PlaylistComponent* playlistComp;
    Toolbar* toolbar;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainComponent (const MainComponent&);
    const MainComponent& operator= (const MainComponent&);
};


#endif   // __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_2BB1072D__
