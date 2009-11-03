#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: PlayerThread.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// A thread used for controlling the music player.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "../System/MessageQueue.h"

#include "PlayerStatus.h"

class Playlist;
class SongInfo;

class PlayerThread : public Thread,
					 public MessageQueue,
					 public ActionBroadcaster
{
public:

	PlayerThread(boost::shared_ptr<Playlist> playlist);
	~PlayerThread(void);

	// Sets the current playlist to use
	//void setPlaylist(boost::shared_ptr<Playlist> playlist);

	// Start playing current playlist
	void play();
	void pause();
	void stop();

	Player::Status getPlayerStatus();
	//Player::Status getWantedState();
	boost::shared_ptr<SongInfo> getCurrentSong();
	uint32 getCurrentSongTime();
	void setCurrentSongTime(uint32 seconds);

	// Thread main loop
	virtual void run();

	void handleMessage(const Message& message);

private:
	class impl;
	impl* _pimpl;
};
