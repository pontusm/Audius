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

class SongInfo;

class AudioPlayer : public DeletedAtShutdown,
					public ActionBroadcaster,
					private ChangeListener
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

	// Sets the current playlist position and optionally starts playing if stopped
	void setCurrentPlaylistPosition(int position, bool startplay);

	Player::Status getPlayerStatus();

	double getCurrentSongPosition();
	double getCurrentSongPositionPercent();
	void setCurrentSongPosition(double position);
	void setCurrentSongPositionPercent(double percent);

	std::shared_ptr<Playlist> getPlaylist();
	std::shared_ptr<SongInfo> getCurrentSong();

	void setPlaylist(std::shared_ptr<Playlist> playlist);

private:
	void changeListenerCallback(ChangeBroadcaster* source);

	void refreshStream();

private:
	class impl;
	impl* vars;
};
