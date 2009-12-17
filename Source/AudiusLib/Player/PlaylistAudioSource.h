#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: PlaylistAudioSource.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// An audio source that ensures that there is a constant stream of audio coming from
// the current playlist. Coordinates downloading from remote services if necessary
// to keep buffers satisfied with data.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class DownloadProgressEventArgs;
class Playlist;
class DownloadThread;

class PlaylistAudioSource :
	public AudioTransportSource
{
public:
	PlaylistAudioSource(void);
	~PlaylistAudioSource(void);

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);

	void setCurrentPlaylist(boost::shared_ptr<Playlist> playlist)
	{
		_playlist = playlist;
	}

private:
	void receiveData(boost::shared_ptr<DownloadProgressEventArgs> args);

private:
	boost::shared_ptr<Playlist>			_playlist;
	boost::shared_ptr<DownloadThread>	_downloadThread;
};
