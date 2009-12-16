#include "Precompiled.h"
#include "AudioPlayer.h"

#include "../System/Exception.h"

#include "../Services/ServiceManager.h"
#include "../Model/ModelBuilder.h"
#include "../Model/SongInfo.h"

#include "../Audio/Mp3AudioFormat.h"

#include "PlaylistAudioSource.h"

#include "ClodderPlaylistFetcher.h"

#include "../mpg123.h"

using namespace libmpg123Namespace;

using namespace boost;

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
		playlist(shared_ptr<Playlist>( new Playlist() )),
		formatReaderSource(NULL)
	{
		// Init mp3 lib
		if(mpg123_init() != MPG123_OK)
			Logger::writeToLog(T("Failed to initialize mp3 library."));
	}
	~impl()
	{
		deleteAndZero(formatReaderSource);

		mpg123_exit();
	}

	shared_ptr<Playlist>	playlist;

	AudioDeviceManager		deviceManager;
	AudioSourcePlayer		sourcePlayer;
	AudioTransportSource	transportSource;
	PlaylistAudioSource		playlistSource;
	
	AudioFormatReaderSource*	formatReaderSource;

	String						clodderKey;
	shared_ptr<ClodderService>	clodder;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
AudioPlayer::AudioPlayer() :
	vars( new impl() )
{
}

AudioPlayer::~AudioPlayer(void)
{
	delete vars;
}

// *** Init **************************************************

void AudioPlayer::initialise()
{
	Logger::writeToLog(T("Initializing player..."));

	String err = vars->deviceManager.initialise(0, 2, NULL, true);
	if(err.length() > 0)
		throw Exception(T("Failed to initialize audio device. ") + err);

	AudioIODevice* device = vars->deviceManager.getCurrentAudioDevice();
	if(device)
	{
		Logger::writeToLog(T("  Found device: ") + device->getName());
		Logger::writeToLog(T("   Device type: ") + device->getTypeName());
		Logger::writeToLog(T("  Audio format: ") +
			String(device->getCurrentSampleRate()) + T("Hz ") +
			String(device->getCurrentBitDepth()) + T(" bits"));
	}
	else
		Logger::writeToLog(T("Unable to find a suitable audio device."));

	AudioDeviceManager::AudioDeviceSetup deviceSettings;
	vars->deviceManager.getAudioDeviceSetup(deviceSettings);
	deviceSettings.bufferSize = 2048;
	err = vars->deviceManager.setAudioDeviceSetup(deviceSettings, true);
	if(err.length() > 0)
		throw Exception(T("Unable to setup audio device. ") + err);

	// Setup audio chain
	vars->deviceManager.addAudioCallback(&vars->sourcePlayer);
	vars->sourcePlayer.setSource(&vars->transportSource);
	//vars->transportSource.setSource(&vars->playlistSource);

	// *** Debug playlist
	//vars->clodder = ServiceManager::getInstance()->getClodder();
	//vars->clodderKey = vars->clodder->login(T("xx"), T("xx"));

	// *** Debug mp3 playing
	FileInputStream* input = new FileInputStream(File(T("D:\\Music\\mp3\\Artists\\R\\Rammstein\\Rammstein - Sehnsucht (1997)\\rammstein-engel.mp3")));
	//FileInputStream* input = new FileInputStream(File(T("D:\\Projects\\Current\\Audius\\test.mp3")));
	Mp3AudioFormat mp3Format;
	AudioFormatReader* reader = mp3Format.createReaderFor(input, false);
	vars->formatReaderSource = new AudioFormatReaderSource(reader, true);
	vars->transportSource.setSource(vars->formatReaderSource, 16384);

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
		vars->transportSource.start();
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

// *** Playlist **************************************************

shared_ptr<SongInfo> AudioPlayer::getCurrentSong()
{
	shared_ptr<PlaylistEntry> entry = vars->playlist->getCurrentEntry();
	if(!entry)
		return shared_ptr<SongInfo>();
	return entry->getSongInfo();
}

shared_ptr<Playlist> AudioPlayer::getPlaylist()
{
	return vars->playlist;
}

void AudioPlayer::goToNext()
{
}

void AudioPlayer::goToPrevious()
{
}

void AudioPlayer::refreshPlaylist()
{
	ClodderPlaylistFetcher fetcher;

	if(fetcher.runThread())
	{
		vars->playlist = fetcher.getPlaylist();
		sendActionMessage(PlayerNotifications::playlistChanged);
	}
}

// Singleton impl
juce_ImplementSingleton(AudioPlayer)
