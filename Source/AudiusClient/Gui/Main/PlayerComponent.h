/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  27 Oct 2009 9:12:48 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_PLAYERCOMPONENT_PLAYERCOMPONENT_C29CEC5F__
#define __JUCER_HEADER_PLAYERCOMPONENT_PLAYERCOMPONENT_C29CEC5F__

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
class PlayerComponent  : public Component,
                         public ActionListener,
                         public Timer,
                         public SliderListener
{
public:
    //==============================================================================
    PlayerComponent (MusicPlayer* player, ApplicationCommandManager* commandManager);
    ~PlayerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	virtual void actionListenerCallback(const String& message);
	virtual void sliderDragEnded(Slider* slider);
	virtual void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);

    // Binary resources:
    static const char* media_play_png;
    static const int media_play_pngSize;
    static const char* media_play_hover_png;
    static const int media_play_hover_pngSize;
    static const char* media_play_down_png;
    static const int media_play_down_pngSize;
    static const char* media_beginning_png;
    static const int media_beginning_pngSize;
    static const char* media_beginning_hover_png;
    static const int media_beginning_hover_pngSize;
    static const char* media_beginning_down_png;
    static const int media_beginning_down_pngSize;
    static const char* media_end_png;
    static const int media_end_pngSize;
    static const char* media_end_hover_png;
    static const int media_end_hover_pngSize;
    static const char* media_end_down_png;
    static const int media_end_down_pngSize;
    static const char* loudspeaker_png;
    static const int loudspeaker_pngSize;
    static const char* media_pause_png;
    static const int media_pause_pngSize;
    static const char* media_pause_hover_png;
    static const int media_pause_hover_pngSize;
    static const char* media_pause_down_png;
    static const int media_pause_down_pngSize;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	MusicPlayer*	_player;
    //[/UserVariables]

    //==============================================================================
    Label* titleLabel;
    Label* artistLabel;
    Label* albumLabel;
    ImageButton* playButton;
    ImageButton* previousButton;
    ImageButton* nextButton;
    Label* timeLabel;
    Slider* songPositionSlider;
    Image* cachedImage_loudspeaker_png;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    PlayerComponent (const PlayerComponent&);
    const PlayerComponent& operator= (const PlayerComponent&);
};


#endif   // __JUCER_HEADER_PLAYERCOMPONENT_PLAYERCOMPONENT_C29CEC5F__
