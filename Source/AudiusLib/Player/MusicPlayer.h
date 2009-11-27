#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: MusicPlayer.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Communicates with the background thread that is handling music playing.
// Wraps different 3rd party libs for supporting various formats.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "../System/MessageQueue.h"

#include "PlayerStatus.h"
#include "PlaylistEntry.h"

class SongInfo;

class MusicPlayer : public Thread,
					public MessageQueue
{
public:
	MusicPlayer(void);
	~MusicPlayer(void);

	void shutdown();

	void startPlaying();
	void pausePlaying();
	void togglePlayPause();
	void stopPlaying();
	void goToNext();
	void goToPrevious();
	void refreshPlaylist();

	Player::Status getPlayerStatus();
	boost::shared_ptr<SongInfo> getCurrentSong();
	uint32 getCurrentSongTime();
	void setCurrentSongTime(uint32 seconds);

	void registerListener(ActionListener* const listener);

	bool isLoggedIn();
	bool performLogin(const String& login, const String& password);

	// Playlist handling
	boost::shared_ptr<PlaylistEntry> getPlaylistEntry(uint32 position);
	uint32 getPlaylistCount();
	uint32 getPlaylistPosition();

	// Thread main loop
	virtual void run();

	void handleMessage(const Message& message);

private:
	class impl;
	impl* _pimpl;
};
