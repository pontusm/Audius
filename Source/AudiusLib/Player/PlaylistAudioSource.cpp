#include "Precompiled.h"
#include "PlaylistAudioSource.h"

#include "Playlist.h"

using namespace boost;

PlaylistAudioSource::PlaylistAudioSource(void)
{
}

PlaylistAudioSource::~PlaylistAudioSource(void)
{
}

void PlaylistAudioSource::prepareToPlay( int samplesPerBlockExpected, double sampleRate )
{
	AudioTransportSource::prepareToPlay(samplesPerBlockExpected, sampleRate);

	if(!_playlist)
		return;

	// Initiate playing from playlist
	shared_ptr<PlaylistEntry> entry = _playlist->getCurrentEntry();
	if(!entry)
		return;

	//DownloadManager::getInstance()->downloadAsync(entry->getUrl())
}
