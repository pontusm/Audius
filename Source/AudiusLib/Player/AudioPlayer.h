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

#include "PlayerStatus.h"
#include "Playlist.h"

class AudioPlayer : public DeletedAtShutdown
{
private:
	AudioPlayer(void);
	~AudioPlayer(void);

public:
	juce_DeclareSingleton(AudioPlayer, true)

	void initialise();
	void shutdown();

	void startPlaying();
	void pausePlaying();
	void togglePlayPause();
	void stopPlaying();
	void goToNext();
	void goToPrevious();
	void refreshPlaylist();

	Player::Status getPlayerStatus();

	boost::shared_ptr<Playlist> getPlaylist();

private:
	class impl;
	impl* vars;
};
