#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: AudioPlayer.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles audio playing both for local files and streaming from remote services.
// Wraps different 3rd party libs for supporting various formats.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class AudioPlayer : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(AudioPlayer, true)

private:
	AudioPlayer(void);
	~AudioPlayer(void);

public:
	void initialize();

private:
	AudioDeviceManager _deviceManager;
};
