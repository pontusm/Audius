#include "Precompiled.h"
#include "PlaylistAudioSource.h"

#include "Playlist.h"

#include "../Downloader/DownloadManager.h"

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
	std::shared_ptr<PlaylistEntry> entry = _playlist->getCurrentEntry();
	if(!entry)
		return;

	//DownloadProgressDelegate callback = boost::bind(&PlaylistAudioSource::receiveData, this, _1);
	//_downloadThread = DownloadManager::getInstance()->downloadAsync(entry->getUrl(), callback);
}

void PlaylistAudioSource::receiveData(std::shared_ptr<DownloadProgressEventArgs> args)
{

}

void PlaylistAudioSource::getNextAudioBlock( const AudioSourceChannelInfo& bufferToFill )
{
	// Plocka från aktuell download buffert
	// Om det inte finns någon buffert, starta download
	// Om download är klar, starta eventuell nästa download

	// StreamingSource laddar från en URL

	AudioTransportSource::getNextAudioBlock(bufferToFill);
}