#include "Precompiled.h"
#include "PlaylistAudioSource.h"

#include "Playlist.h"

#include "../Downloader/DownloadManager.h"
#include "../Downloader/DownloadThread.h"

using namespace boost;

PlaylistAudioSource::PlaylistAudioSource(void)
{
}

PlaylistAudioSource::~PlaylistAudioSource(void)
{
}

void PlaylistAudioSource::receiveData(shared_ptr<DownloadProgressEventArgs> args)
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

	DownloadProgressDelegate callback = boost::bind(&PlaylistAudioSource::receiveData, this, _1);
	_downloadThread = DownloadManager::getInstance()->downloadAsync(entry->getUrl(), callback);
}
