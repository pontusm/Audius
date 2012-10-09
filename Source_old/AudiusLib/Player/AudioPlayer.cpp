#include "Precompiled.h"
#include "AudioPlayer.h"

#include "../System/Exception.h"

#include "../Services/ServiceManager.h"
#include "../Model/ModelBuilder.h"
#include "../Model/SongInfo.h"

#include "../Audio/Mp3AudioFormat.h"

#include "../Downloader/DownloadManager.h"
#include "../Downloader/DownloadStream.h"

#include "PlaylistAudioSource.h"
#include "StreamingAudioSource.h"

#include "ClodderPlaylistFetcher.h"

#include "../mpg123.h"

using namespace libmpg123Namespace;

// ********************
// *** Private data ***
// ********************
class AudioPlayer::impl
{
private:
	impl(impl const &);
	impl& operator=(impl const &);

public:
	impl() :
		playlist(std::shared_ptr<Playlist>( new Playlist() )),
		streamingAudioSource(NULL)
	{
	}
	~impl()
	{
		deleteAndZero(streamingAudioSource);
	}

	std::shared_ptr<Playlist>	playlist;
	std::shared_ptr<SongInfo>	currentSong;

	AudioDeviceManager		deviceManager;
	AudioSourcePlayer		sourcePlayer;
	AudioTransportSource	transportSource;
	
	StreamingAudioSource*	streamingAudioSource;
	
	Mp3AudioFormat	mp3Format;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
AudioPlayer::AudioPlayer() :
	vars( new impl() )
{
	// Init mp3 lib
	if(mpg123_init() != MPG123_OK)
		Log::write("Failed to initialize mp3 library.");
}

AudioPlayer::~AudioPlayer(void)
{
	mpg123_exit();
	delete vars;
	clearSingletonInstance();
}

// *** Init **************************************************

void AudioPlayer::initialise()
{
	Log::write("Initializing player...");

	String err = vars->deviceManager.initialise(0, 2, NULL, true);
	if(err.length() > 0)
		throw Exception("Failed to initialize audio device. " + err);

	AudioIODevice* device = vars->deviceManager.getCurrentAudioDevice();
	if(device)
	{
		Log::write("  Found device: " + device->getName());
		Log::write("   Device type: " + device->getTypeName());
		Log::write("  Audio format: " +
			String(device->getCurrentSampleRate()) + "Hz " +
			String(device->getCurrentBitDepth()) + " bits");
	}
	else
		Log::write("Unable to find a suitable audio device.");

	AudioDeviceManager::AudioDeviceSetup deviceSettings;
	vars->deviceManager.getAudioDeviceSetup(deviceSettings);
	//deviceSettings.bufferSize = 2048;
	//deviceSettings.bufferSize = 8192;
	err = vars->deviceManager.setAudioDeviceSetup(deviceSettings, true);
	if(err.length() > 0)
		throw Exception("Unable to setup audio device. " + err);

	// Setup audio chain
	vars->deviceManager.addAudioCallback(&vars->sourcePlayer);
	vars->sourcePlayer.setSource(&vars->transportSource);
	//vars->transportSource.setSource(&vars->playlistSource);

	vars->transportSource.addChangeListener(this);
}

// *** Shutdown **************************************************

void AudioPlayer::shutdown()
{
	vars->deviceManager.removeAudioCallback(&vars->sourcePlayer);

	vars->transportSource.setSource(NULL);
	vars->sourcePlayer.setSource(NULL);
	vars->deviceManager.closeAudioDevice();
}

// *** Play control **************************************************

void AudioPlayer::startPlaying()
{
	if(!vars->transportSource.isPlaying())
		vars->transportSource.start();
}

void AudioPlayer::pausePlaying()
{
	if(vars->transportSource.isPlaying())
		vars->transportSource.stop();
}

void AudioPlayer::togglePlayPause()
{
	if(vars->transportSource.isPlaying())
		vars->transportSource.stop();
	else
	{
		if(!vars->streamingAudioSource)
			refreshStream();
		vars->transportSource.start();
	}
}

void AudioPlayer::stopPlaying()
{
	if(vars->transportSource.isPlaying())
		vars->transportSource.stop();
	vars->transportSource.setPosition(0);
}

Player::Status AudioPlayer::getPlayerStatus()
{
	if(vars->transportSource.isPlaying())
		return Player::Playing;
	else if(vars->transportSource.hasStreamFinished())
		return Player::Stopped;
	else if(!vars->transportSource.isPlaying())
		return Player::Paused;
	else
		return Player::Unknown;
}

double AudioPlayer::getCurrentSongPosition()
{
	return vars->transportSource.getCurrentPosition();
}

void AudioPlayer::setCurrentSongPosition( double position )
{
	vars->transportSource.setPosition(position);
}

double AudioPlayer::getCurrentSongPositionPercent()
{
	std::shared_ptr<SongInfo> songInfo = getCurrentSong();
	int seconds = songInfo->getLengthSeconds();
	if(seconds <= 0)
		return 0;

	return vars->transportSource.getCurrentPosition() / (double)seconds;
}

void AudioPlayer::setCurrentSongPositionPercent( double percent )
{
	jassert(percent >= 0 && percent <= 1);

	std::shared_ptr<SongInfo> songInfo = getCurrentSong();
	int seconds = songInfo->getLengthSeconds();
	if(seconds <= 0)
		return;

	vars->transportSource.setPosition(seconds * percent);
}

// *** Playlist **************************************************

std::shared_ptr<SongInfo> AudioPlayer::getCurrentSong()
{
	return vars->currentSong;
}

std::shared_ptr<Playlist> AudioPlayer::getPlaylist()
{
	return vars->playlist;
}


void AudioPlayer::setPlaylist( std::shared_ptr<Playlist> playlist )
{
	vars->playlist = playlist;
	refreshStream();
	sendActionMessage(PlayerNotifications::playlistChanged);
}

void AudioPlayer::setCurrentPlaylistPosition( int position, bool startplay )
{
	if(!vars->playlist->setCurrentPosition(position))
		return;

	refreshStream();

	if(startplay)
		startPlaying();
}

void AudioPlayer::goToNext()
{
	if(!vars->playlist->gotoNextEntry())
		return;

	refreshStream();
}

void AudioPlayer::goToPrevious()
{
	if(!vars->playlist->gotoPreviousEntry())
		return;

	refreshStream();
}

void AudioPlayer::refreshPlaylist()
{
	ClodderPlaylistFetcher fetcher;

	if(fetcher.runThread())
	{
		vars->playlist = fetcher.getPlaylist();
		sendActionMessage(PlayerNotifications::serverPlaylistUpdated);
		sendActionMessage(PlayerNotifications::playlistChanged);
	}
}

void AudioPlayer::refreshStream()
{
	bool playing = getPlayerStatus() == Player::Playing;

	// Detach current stream and discard it
	if(vars->streamingAudioSource)
	{
		vars->transportSource.setSource(0);
		deleteAndZero(vars->streamingAudioSource);
	}

	// Update current song from playlist
	std::shared_ptr<PlaylistEntry> entry = vars->playlist->getCurrentEntry();
	if(!entry)
		return;
	vars->currentSong = entry->getSongInfo();
	if(!vars->currentSong)
		return;

	Log::write("Playing song: " + vars->currentSong->getArtist() + " \"" + vars->currentSong->getTitle() + "\"");

	String url = ServiceManager::getInstance()->getClodder()->getSongUrl(vars->currentSong->getSongID());
	vars->streamingAudioSource = new StreamingAudioSource(url, vars->mp3Format);
	vars->streamingAudioSource->getStream()->addChangeListener(this);
	vars->transportSource.setSource(vars->streamingAudioSource);

	if(playing)
		vars->transportSource.start();

	sendActionMessage(PlayerNotifications::newSong);
}

// *** Change listener **************************************************

void AudioPlayer::changeListenerCallback(ChangeBroadcaster* source)
{
	// Transport changed?
	if(source == &vars->transportSource)
	{
		if(vars->transportSource.hasStreamFinished())
		{
			// Stream is completed so it's time for the next track
			if(!vars->playlist->gotoNextEntry())
			{
				Log::write("End of playlist reached.");
				return;
			}

			// Setup track and start playing
			refreshStream();
			vars->transportSource.start();
		}
	}
	else if(source == vars->streamingAudioSource->getStream())
	{
		DownloadStream* stream = vars->streamingAudioSource->getStream();
		//if(stream->isDownloadComplete() && vars->currentSong->getLengthSeconds() <= 0)
		if(stream->isDownloadComplete())
		{
			// Length is unknown so we need to analyze the song and determine the true length
			AudioFormatReader* reader = vars->mp3Format.createReaderFor(
											new MemoryInputStream(stream->getData(), stream->getTotalLength(), false), true);
			if(reader)
			{
				double samplerate = vars->deviceManager.getCurrentAudioDevice()->getCurrentSampleRate();
				double seconds = reader->lengthInSamples / samplerate;
				vars->currentSong->setLengthSeconds(seconds);
				delete reader;

				// Signal to UI that song updated
				sendActionMessage(PlayerNotifications::songInfoChanged);
			}
		}
	}
}

// Singleton impl
juce_ImplementSingleton(AudioPlayer)
