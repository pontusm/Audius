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
class WebRequest;

class PlaylistAudioSource :
	public AudioTransportSource
{
public:
	PlaylistAudioSource(void);
	~PlaylistAudioSource(void);

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

	void setCurrentPlaylist(std::shared_ptr<Playlist> playlist)
	{
		_playlist = playlist;
	}

private:
	void receiveData(std::shared_ptr<DownloadProgressEventArgs> args);

private:
	std::shared_ptr<Playlist>		_playlist;
	std::shared_ptr<WebRequest>	_request;
};
