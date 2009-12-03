#include "Precompiled.h"
#include "PlayerThread.h"

#include "../System/Exception.h"

#include "../Model/SongInfo.h"

#include "../Downloader/DownloadManager.h"
#include "../Downloader/DownloadProgressEventArgs.h"
#include "../Downloader/DownloadThread.h"

#include "Playlist.h"
#include "PlaylistEntry.h"

using namespace boost;

#define MP3_BUFFER_LENGTH_MS	2000
#define MP3_FADEOUT_TIME_MS		200
#define MAX_STREAM_FAIL_COUNT	20

// Internal messaging class
class PlayerThreadMessage : public Message
{
public:
	enum Command
	{
		Play,
		Pause,
		Stop,
		Seek
	};

	PlayerThreadMessage(Command cmd) :
		command(cmd)
	{
	}

	Command command;

	//shared_ptr<Playlist> playlist;
	uint32	songPositionSeconds;
};

// ******************************
// *** Private implementation ***
// ******************************
class PlayerThread::impl
{
private:
	impl(impl const &);
	impl& operator=(impl const &);

public:

	impl(shared_ptr<Playlist> playlist) :
	  _playlist(playlist),
	  _isBuffering(false),
	  _isDownloading(false),
	  _isNewStream(true)
	{
		//_player = CreateCWMp3();
	}
	~impl()
	{
		//_player->Release();
	}

	// This is called periodically to handle routine tasks
	void processCurrentState()
	{
		// Do we need to switch state?
		verifyCurrentState();

		switch(getPlayerStatus())
		{
		case Player::Playing:
			handlePlaying();
			break;
		}
	}

	// Retrieves the current status for the player
	Player::Status getPlayerStatus()
	{
		const ScopedLock l(_lock);

		// Check player status
/*		MP3_STATUS status;
		_player->GetStatus(&status);
		//if(_isBuffering)
		//	return Player::Buffering;
		if(status.fPlay)
			return Player::Playing;
		if(status.fPause)
			return Player::Paused;
		if(status.fStop)
			return Player::Stopped;
*/
		return Player::Unknown;
	}

	shared_ptr<SongInfo> getCurrentSong()
	{
		const ScopedLock l(_lock);
		return _currentSong;
	}

	uint32 getCurrentSongTime()
	{
		const ScopedLock l(_lock);
/*		MP3_TIME position;
		_player->GetPosition(&position);
		return position.sec;
*/
		return 0;
	}

	// Called when user wants to play
	void play()
	{
		// Already playing?
		if(getPlayerStatus() == Player::Playing)
			return;

		const ScopedLock l(_lock);

		// Check player status
/*		MP3_STATUS status;
		_player->GetStatus(&status);

		// Resume paused song?
		if(status.fPause)
		{
			//_currentState = Playing;
			_player->Resume();
			return;
		}

		startPlaying();
*/
	}

	// Called when music should pause
	void pause()
	{
		const ScopedLock l(_lock);

		// Check player status
/*		MP3_STATUS status;
		_player->GetStatus(&status);
		if(status.fPlay)
			_player->Pause();
*/
	}

	// Called when music should stop
	void stop()
	{
		// Abort any ongoing download (must be done before entering critical section)
		abortDownload();

		const ScopedLock l(_lock);

		// Check player status
/*		MP3_STATUS status;
		_player->GetStatus(&status);
		if(!status.fStop)
			_player->Stop();
*/
	}

	void seek(uint32 seconds)
	{
		const ScopedLock l(_lock);
/*		MP3_TIME position;
		position.sec = seconds;
		if(_player->Seek(TIME_FORMAT_SEC, &position, SONG_BEGIN) == 0)
		{
			String err(_player->GetError());
			Logger::writeToLog("Seek failed: " + err);
			return;
		}
*/
		// Resume playing
		play();
	}

	// Private methods
private:
	void verifyCurrentState()
	{
		// Are we in the correct state?
/*		Player::Status status = getPlayerStatus();
		if(status == _wantedState)
			return;

		// Try to switch to the wanted state
		switch(_wantedState)
		{
		case Player::Stopped:
			stop();
			break;
		case Player::Paused:
			// We cannot move from stopped to paused
			if(status != Player::Stopped)
				pause();
			else
				_wantedState = status;		// We're already stopped
			break;
		case Player::Playing:
			// If not already buffering, try to start playing
			//if(status != Player::Buffering)
			//	startPlaying();
			break;
		}
*/
	}

	// This is called periodically when in play mode
	void handlePlaying()
	{
		const ScopedLock l(_lock);

		// Skip processing if buffering or if we have no song
		if(_isBuffering || !_currentSong)
			return;

		// No song loaded?
		//if(!_currentSong)
		//{
		//	// Any current entry to play?
		//	shared_ptr<PlaylistEntry> playlistEntry = _playlist->getCurrentEntry();
		//	if(playlistEntry == NULL)
		//	{
		//		Logger::writeToLog(T("Unable to start playing. Playlist is empty."));
		//		return;
		//	}

		//	_currentSong = playlistEntry->getSongInfo();
		//}

		// Check if we have reached the end of the current song
		uint32 length = _currentSong->getLengthSeconds();

/*		MP3_TIME position;
		_player->GetPosition(&position);

		// Not even started yet?
		if(position.ms <= 0)
			return;

		// Check if stopped by comparing last known position
		if(_lastKnownPosition != position.ms)
		{
			// We've not stopped yet
			_lastKnownPosition = position.ms;
			_songStopped = false;
			return;
		}

		// Just detected that song stopped?
		if(!_songStopped)
		{
			// Record time of stop detected
			_songStopped = true;
			_songStoppedTime = Time::getCurrentTime();
			return;
		}

		// Song has stopped. This indicates end of song IF we are within the last
		// 10 seconds of the known length of the song (actual play length can differ a few secs from server)
		if(position.sec < length - 10)
		{
			// We've detected that song has stopped, but according to the song length from the server
			// we should not yet be at the end. We wait a few extra seconds to ensure we're not switching prematurely.
			// This fixes problems with invalid song length that is present in some songs.
			RelativeTime diff = Time::getCurrentTime() - _songStoppedTime;
			if(diff.inSeconds() < 2)
				return;
		}

		// Step to next song
		if(_playlist->gotoNextEntry())
		{
			// Since we are in a critical section here we will deadlock if we try to break the
			// ongoing download. Instead we post a play message for later processing to resume playing.
			_player->Stop();
			_ownerThread->postThreadMessage( new PlayerThreadMessage(PlayerThreadMessage::Play) );
		}
		else
		{
			// Seems we've reached the end
			Logger::writeToLog(T("End of playlist reached."));
			_ownerThread->postThreadMessage( new PlayerThreadMessage(PlayerThreadMessage::Stop) );
		}
*/
	}

	// Starts buffering/playing the current song in the playlist
	void startPlaying()
	{
		// Abort any ongoing download
		// Stop it before entering critical section (to avoid deadlock)
		abortDownload();

		const ScopedLock l(_lock);

		// Any current entry to play?
		shared_ptr<PlaylistEntry> playlistEntry = _playlist->getCurrentEntry();
		if(playlistEntry == NULL)
		{
			Logger::writeToLog(T("Unable to start playing. Playlist is empty."));
			return;
		}

		_currentSong = playlistEntry->getSongInfo();
		Logger::writeToLog(String(T("Playing song '")) + _currentSong->getTitle() + String(T("' - ") + _currentSong->getArtist()));

		_ownerThread->sendActionMessage(PlayerNotifications::newSong);

		// TODO: Lookup song in cache

		// Start downloading
		//_currentState = Buffering;
		_isNewStream = true;
		_isBuffering = true;
		_isDownloading = true;
		_streamFailCount = 0;

		DownloadProgressDelegate callback = bind(&impl::receiveData, this, _1);
		_downloadThread = DownloadManager::getInstance()->downloadAsync(playlistEntry->getUrl(), callback);
	}

	// Downloaded data will be received by this method (called by the download thread)
	void receiveData(shared_ptr<DownloadProgressEventArgs> args)
	{
		// Discard data not buffering and player is stopped (or if fail count is too high)
		if((!_isBuffering && getPlayerStatus() == Player::Stopped) || _streamFailCount > MAX_STREAM_FAIL_COUNT)
			return;

		int result = 0;

		// Enter critical section
		const ScopedLock l(_lock);
/*
		// Stream data
		if(_isNewStream)
		{
			result = _player->OpenStream(1, 1, args->buffer, args->bufferSize, MP3_BUFFER_LENGTH_MS);
			if(result != 0)
			{
				_isNewStream = false;
			}
			else
			{
				// Failed to open stream, but we will keep trying when next data block is received
				String err(_player->GetError());
				Logger::writeToLog("OpenStream failed: " + err);

				_streamFailCount++;
				if(_streamFailCount >= MAX_STREAM_FAIL_COUNT)
				{
					Logger::writeToLog("Too many errors, skipping to next song.");

					// We've tried opening this stream too many times. Skip to the next song.
					_downloadThread->signalThreadShouldExit();

					// Stop buffering, clear current song and advance to next song in playlist
					// This will eventually make player try to resume playing the next song
					_currentSong = shared_ptr<SongInfo>();
					_playlist->gotoNextEntry();
					_isBuffering = false;
					_streamFailCount = 0;
				}
			}
		}
		else
		{
			result = _player->PushDataToStream(args->buffer, args->bufferSize);
			//if(result == 0)
			//{
			//	String err(_player->GetError());
			//	DBG(T("PushDataToStream failed: ") + err);
			//}
		}

		// Enough data to begin playing?
		if(!_isNewStream && _isBuffering && args->secondsLeft < _currentSong->getLengthSeconds())
		{
			// No need to buffer anymore, start playing
			_isBuffering = false;

			_player->Play();

			// Fade in volume
			MP3_TIME fadetime;
			fadetime.ms = 100;
			_player->FadeVolume(FADE_IN, TIME_FORMAT_MS, &fadetime);
		}

		_isDownloading = !args->isFinished;
*/
	}

	void fadeOut(bool wait)
	{
/*		MP3_STATUS status;
		_player->GetStatus(&status);
		if(!status.fPlay)
			return;

		DBG(T("Fading out..."));

		// Fade out volume
		MP3_TIME fadetime;
		fadetime.ms = MP3_FADEOUT_TIME_MS;
		_player->FadeVolume(FADE_OUT, TIME_FORMAT_MS, &fadetime);
		if(wait)
			Thread::getCurrentThread()->wait(MP3_FADEOUT_TIME_MS + MP3_BUFFER_LENGTH_MS);
*/
	}

	// Aborts any download in progress
	// This should not be done in a critical section because it will cause deadlock
	void abortDownload()
	{
		if(_downloadThread && _downloadThread->isThreadRunning())
		{
			_downloadThread->signalThreadShouldExit();
			if(!_downloadThread->waitForThreadToExit(2000))
			{
				DBG(T("Failed to stop thread! Possible deadlock!"));
			}
			//_downloadThread->stopThread(10000);
		}
	}

public:
	PlayerThread*	_ownerThread;

private:
	shared_ptr<Playlist>		_playlist;
	shared_ptr<DownloadThread>	_downloadThread;

	// These variables are accessed by both Player and Download threads
	// Must only be accessed within critical section
	CriticalSection			_lock;
	shared_ptr<SongInfo>	_currentSong;
	//CWMp3*					_player;

	uint32	_lastKnownPosition;
	bool	_songStopped;
	Time	_songStoppedTime;

	// No lock is required for these primitives
	volatile bool	_isNewStream;
	volatile bool	_isDownloading;
	volatile bool	_isBuffering;
	//volatile bool	_abortDownload;
	//volatile State	_currentState;
	volatile uint8	_streamFailCount;

	//volatile Player::Status	_wantedState;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
PlayerThread::PlayerThread( shared_ptr<Playlist> playlist ) :
	Thread(T("Player")),
	_pimpl( new impl(playlist) )
{
	_pimpl->_ownerThread = this;
}

PlayerThread::~PlayerThread( void )
{
	delete _pimpl;
}

// ** Thread entry point **
void PlayerThread::run()
{
	try
	{
		Logger::writeToLog(T("Player thread started"));
		while(!threadShouldExit())
		{
			// Process all pending messages
			processPendingMessages();
	
			// Let private class process current state
			_pimpl->processCurrentState();
	
			// Make a small delay to avoid hogging all CPU
			sleep(20);
		}
		Logger::writeToLog(T("Player thread stopped"));
	}
	catch (Exception& ex)
	{
		// Exception is expected when thread is being aborted
		if(!threadShouldExit())
			Logger::writeToLog(T("Player thread exception: ") + ex.getFullMessage());
	}
}

// ** Handle messages posted to player thread **
void PlayerThread::handleMessage( const Message& message )
{
	try
	{
		const PlayerThreadMessage* msg = dynamic_cast<const PlayerThreadMessage*>(&message);
		if(msg == NULL)
			return;

		switch(msg->command)
		{
		case PlayerThreadMessage::Play:
			_pimpl->play();
			break;
		case PlayerThreadMessage::Pause:
			_pimpl->pause();
			break;
		case PlayerThreadMessage::Stop:
			_pimpl->stop();
			break;
		case PlayerThreadMessage::Seek:
			_pimpl->seek(msg->songPositionSeconds);
			break;
		}
	}
	catch (Exception& ex)
	{
		Logger::writeToLog(T("PlayerThread failed to handle command. " + ex.getFullMessage()));
	}
}


// ********************************
// *** Public interface methods ***
// ********************************

// These methods are called by foreign threads (usually the UI thread)
// To keep things clean they simply post messages to the player thread for later processing

void PlayerThread::play()
{
	postThreadMessage( new PlayerThreadMessage(PlayerThreadMessage::Play) );
}

void PlayerThread::pause()
{
	postThreadMessage( new PlayerThreadMessage(PlayerThreadMessage::Pause) );
}

void PlayerThread::stop()
{
	postThreadMessage( new PlayerThreadMessage(PlayerThreadMessage::Stop) );
}

Player::Status PlayerThread::getPlayerStatus()
{
	return _pimpl->getPlayerStatus();
}

shared_ptr<SongInfo> PlayerThread::getCurrentSong()
{
	return _pimpl->getCurrentSong();
}

uint32 PlayerThread::getCurrentSongTime()
{
	return _pimpl->getCurrentSongTime();
}

void PlayerThread::setCurrentSongTime( uint32 seconds )
{
	PlayerThreadMessage* msg = new PlayerThreadMessage(PlayerThreadMessage::Seek);
	msg->songPositionSeconds = seconds;
	postThreadMessage(msg);
}

//Player::Status PlayerThread::getWantedState()
//{
//	return _pimpl->getWantedState();
//}
