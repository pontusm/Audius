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

#ifndef __JUCER_HEADER_SEARCHCOMPONENT_SEARCHCOMPONENT_FF7F3F47__
#define __JUCER_HEADER_SEARCHCOMPONENT_SEARCHCOMPONENT_FF7F3F47__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"

class SongInfo;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SearchComponent  : public Component,
                         public TableListBoxModel,
                         public TextEditorListener,
                         public ButtonListener,
						 public ActionListener
{
public:
    //==============================================================================
    SearchComponent ();
    ~SearchComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	// TableListBoxModel
	virtual int getNumRows();
	virtual void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);
	virtual void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
	virtual void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& e);

	// TextEditorListener
	virtual void textEditorTextChanged(TextEditor & editor);
	virtual void textEditorReturnKeyPressed(TextEditor & editor);
	virtual void textEditorEscapeKeyPressed(TextEditor & editor);
	virtual void textEditorFocusLost(TextEditor & editor);

	// ActionListener
	virtual void actionListenerCallback(const String& message);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Font _font;
	Font _boldfont;

	std::vector< std::shared_ptr<SongInfo> > _searchResult;

	void doSearch();
    //[/UserVariables]

    //==============================================================================
    TextButton* searchButton;
    TextEditor* textEditor;
    TableListBox* searchlistTable;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    SearchComponent (const SearchComponent&);
    const SearchComponent& operator= (const SearchComponent&);
};


#endif   // __JUCER_HEADER_SEARCHCOMPONENT_SEARCHCOMPONENT_FF7F3F47__
