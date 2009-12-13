#include "Precompiled.h"
#include "AudioPlayer.h"

#include "../System/Exception.h"

#include "../Services/ServiceManager.h"
#include "../Model/ModelBuilder.h"
#include "../Model/SongInfo.h"

#include "../Audio/Mp3AudioFormat.h"

#include "PlaylistAudioSource.h"

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

shared_ptr<PlaylistEntry> createPlaylistEntryFromServerXml(const String & userKey, const String& itemXml)
{
	ModelBuilder builder;
	shared_ptr<SongInfo> songInfo = builder.createSongInfo(itemXml);
	if(!songInfo)
		return shared_ptr<PlaylistEntry>();

	String url = ServiceManager::getInstance()->getClodder()->getSongUrl(userKey, songInfo->getSongID());
	if(url.length() == 0)
		return shared_ptr<PlaylistEntry>();

	DBG(String(T("Scanned song '")) + songInfo->getTitle() + String(T("' - ") + songInfo->getArtist()));

	return shared_ptr<PlaylistEntry>( new PlaylistEntry(songInfo, url) );
}

// Retrieves the current song from server and creates a PlaylistEntry for it
shared_ptr<PlaylistEntry> getCurrentPlaylistEntryFromServer(const String & userKey)
{
	String itemXml = ServiceManager::getInstance()->getClodder()->getCurrentPlaylistItem(userKey);
	return createPlaylistEntryFromServerXml(userKey, itemXml);
}

// *** Init/shutdown **************************************************

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
	deviceSettings.bufferSize = 4096;
	err = vars->deviceManager.setAudioDeviceSetup(deviceSettings, true);
	if(err.length() > 0)
		throw Exception(T("Unable to setup audio device. ") + err);

	// Setup audio chain
	//vars->transportSource.setSource(&vars->playlistSource);
	vars->sourcePlayer.setSource(&vars->transportSource);
	vars->deviceManager.addAudioCallback(&vars->sourcePlayer);

	// *** Debug playlist
	//vars->clodder = ServiceManager::getInstance()->getClodder();
	//vars->clodderKey = vars->clodder->login(T("xx"), T("xx"));

	// *** Debug mp3 playing
	FileInputStream* input = new FileInputStream(File(T("D:\\Music\\mp3\\_incoming\\_Incomplete\\Godspeed You Black Emperor! - Lift Your Skinny Fists Like Antennas To Heaven\\Godspeed You Black Emperor! - 01 - Storm.mp3")));
	Mp3AudioFormat mp3Format;
	AudioFormatReader* reader = mp3Format.createReaderFor(input, false);
	vars->formatReaderSource = new AudioFormatReaderSource(reader, true);
	vars->transportSource.setSource(vars->formatReaderSource);

}

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

// Singleton impl
juce_ImplementSingleton(AudioPlayer)
