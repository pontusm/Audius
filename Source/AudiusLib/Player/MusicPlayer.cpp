#include "Precompiled.h"
#include "MusicPlayer.h"

#include "PlayerThread.h"
#include "Playlist.h"
#include "PlaylistEntry.h"

#include "../Services/KejkService.h"
#include "../Model/ModelBuilder.h"
#include "../Model/SongInfo.h"

#include "../Downloader/DownloadManager.h"

#include "../System/Net/WebException.h"

using namespace boost;

// Internal messaging class
class PlayerMessage : public Message
{
public:
	enum Command
	{
		Play,
		Pause,
		TogglePlayPause,
		Stop,
		Next,
		Previous,
		RefreshPlaylist
	};

	PlayerMessage(Command cmd) :
		command(cmd)
	{
	}

	Command command;
};

// ******************************
// *** Private implementation ***
// ******************************
class MusicPlayer::impl
{
private:
	impl(impl const &);
	impl& operator=(impl const &);

public:
	impl() :
	  _playlist(shared_ptr<Playlist>( new Playlist() ))
	{
		_playerThread = new PlayerThread(_playlist);
		_playerThread->startThread();
	}
	~impl()
	{
		_playerThread->stopThread(5000);
		delete _playerThread;
	}

	// Called from background thread
	void handleMessage(const Message& message)
	{
		//DBG_PRINTF(T("%d"), GetCurrentThreadId());
		
		try
		{
			const PlayerMessage* msg = dynamic_cast<const PlayerMessage*>(&message);
			if(msg == NULL)
				return;
	
			switch(msg->command)
			{
			case PlayerMessage::Play:
				handlePlay();
				break;
			case PlayerMessage::Pause:
				_playerThread->pause();
				break;
			case PlayerMessage::TogglePlayPause:
				if(_playerThread->getPlayerStatus() != Player::Playing)
					handlePlay();
				else
					_playerThread->pause();
				break;
			case PlayerMessage::Stop:
				_playerThread->stop();
				break;
			case PlayerMessage::Next:
				handleGotoNext();
				break;
			case PlayerMessage::Previous:
				handleGotoPrevious();
				break;
			case PlayerMessage::RefreshPlaylist:
				Player::Status status = _playerThread->getPlayerStatus();

				// Stop player and clear playlist
				_playerThread->stop();
				_playlist->clear();

				// Resume playing if user was playing before
				if(status == Player::Playing)
					handlePlay();
				break;
			}
		}
		catch (Exception& ex)
		{
			Logger::writeToLog(T("Failed to handle command. " + ex.getFullMessage()));
		}
	}

	// Perform a login (usually called directly from the UI thread)
	bool performLogin(const String& login, const String& password)
	{
		setCredentials(login, password);
		return ensureLoggedIn();
	}

	bool isLoggedIn()
	{
		const ScopedLock l(_loginLock);
		return _userKey.length() > 0;
	}

	// Private methods
private:
	bool ensureLoggedIn()
	{
		const ScopedLock l(_loginLock);

		// Have user key?
		if(_userKey.length() > 0)
			return true;

		// No credentials set?
		if(_login.length() == 0)
			return false;

		try
		{
			_userKey = _kejkService.login(_login, _password);
			return _userKey.length() > 0;
		}
		catch(WebException& ex)
		{
			_userKey = String::empty;
			Logger::writeToLog(T("Unable to login. " + ex.getMessage()));
			return false;
		}
	}

	void setCredentials(const String& login, const String& password)
	{
		const ScopedLock l(_loginLock);

		_login = login;
		_password = password;
	}

	// Handle the play command
	void handlePlay()
	{
		//if(_playerThread->getPlayerStatus() == Player::Playing)
		//	return;

		// Have a playlist to use?
		if(_playlist->getCount() > 0)
		{
			_playerThread->play();		// Resume playing it
			return;
		}

		// Retrieve current entry from Kejk and start playing it
		shared_ptr<PlaylistEntry> playlistEntry = getCurrentPlaylistEntryFromKejk();
		if(!playlistEntry)
		{
			Logger::writeToLog(T("Unable to start playing. Kejk playlist is empty."));
			return;
		}

		// Start playing
		_playlist->clear();
		_playlist->add( playlistEntry );
		_playerThread->play();

		// Schedule a full scan of the playlist
		// TODO: Schedule in the future to avoid hogging connection and UI thread
		//postMessage( new PlayerMessage(PlayerMessage::RefreshPlaylist) );
		refreshPlaylist();
	}

	// Scans the entire playlist from Kejk
	void refreshPlaylist() 
	{
		if(!ensureLoggedIn())
			return;

		shared_ptr<PlaylistEntry> playlistEntry;
		DBG(T("Scanning playlist..."));

		// Scan forward to get last part of playlist
		do
		{
			playlistEntry = createPlaylistEntryFromKejkXml( _kejkService.gotoNext(_userKey) );
			if(playlistEntry)
				_playlist->add(playlistEntry);
		} while(playlistEntry);

		// Step back to original position
		for(int i = 0; i < (int)_playlist->getCount() - 1; i++)
			_kejkService.gotoPrevious(_userKey);

		// Scan backwards to build first part of playlist
		// ** Temporarily disabling this for now ***
		/*
		DBG(T("Backtracking playlist..."));
		do
		{
		playlistEntry = createPlaylistEntryFromKejkXml( _kejkService.gotoPrevious(_userKey) );
		if(playlistEntry)
		_playlist->insert(0, playlistEntry);
		} while (playlistEntry);
		*/
	}

	// Retrieves the current song from Kejk and creates a PlaylistEntry for it
	shared_ptr<PlaylistEntry> getCurrentPlaylistEntryFromKejk()
	{
		String itemXml = _kejkService.getCurrentPlaylistItem(_userKey);
		return createPlaylistEntryFromKejkXml(itemXml);
	}

	shared_ptr<PlaylistEntry> createPlaylistEntryFromKejkXml(const String& itemXml)
	{
		ModelBuilder builder;
		shared_ptr<SongInfo> songInfo = builder.createSongInfo(itemXml);
		if(!songInfo)
			return shared_ptr<PlaylistEntry>();

		String url = _kejkService.getSongUrl(_userKey, songInfo->getSongID());
		if(url.length() == 0)
			return shared_ptr<PlaylistEntry>();

		DBG(String(T("Scanned song '")) + songInfo->getTitle() + String(T("' - ") + songInfo->getArtist()));

		return shared_ptr<PlaylistEntry>( new PlaylistEntry(songInfo, url) );
	}

	void handleGotoNext()
	{
		// At end of playlist?
		if(_playlist->getCurrentPosition() >= _playlist->getCount() - 1)
		{
			// Retrieve next from Kejk
			if(!ensureLoggedIn())
				return;

			shared_ptr<PlaylistEntry> playlistEntry = createPlaylistEntryFromKejkXml( _kejkService.gotoNext(_userKey) );
			if(!playlistEntry)
				return;

			_playlist->add( playlistEntry );
		}

		// Step to the next playlist entry
		if(!_playlist->gotoNextEntry())
			return;

		if(_playerThread->getPlayerStatus() == Player::Playing)
		{
			// Restart player to pick up next song to play
			_playerThread->stop();
			_playerThread->play();
		}
	}

	void handleGotoPrevious()
	{
		// At start of playlist?
		if(_playlist->getCurrentPosition() <= 0)
		{
			// Check if we can retrieve earlier entries from Kejk
			if(!ensureLoggedIn())
				return;

			shared_ptr<PlaylistEntry> playlistEntry = createPlaylistEntryFromKejkXml( _kejkService.gotoPrevious(_userKey) );
			if(!playlistEntry)
				return;

			_playlist->insert(0, playlistEntry );
		}

		// Step to the previous playlist entry
		if(!_playlist->gotoPreviousEntry())
			return;

		if(_playerThread->getPlayerStatus() == Player::Playing)
		{
			// Restart player to pick up next song to play
			_playerThread->stop();
			_playerThread->play();
		}
	}

	// Public variables
public:
	PlayerThread*	_playerThread;

	// Private variables
private:
	shared_ptr<Playlist>	_playlist;		// Shared with player thread

	KejkService		_kejkService;
	String			_userKey;

	CriticalSection		_loginLock;
	String				_login;
	String				_password;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
MusicPlayer::MusicPlayer() :
	Thread(T("Music controller")),
	_pimpl( new impl() )
{
	startThread();
}

MusicPlayer::~MusicPlayer()
{
	shutdown();

	delete _pimpl;
}

// ** Thread entry point **
void MusicPlayer::run()
{
	try
	{
		Logger::writeToLog(T("Music controller started"));
		while(!threadShouldExit())
		{
			// Process all pending messages
			processPendingMessages();
	
			// Make a small delay to avoid hogging all CPU
			sleep(20);
		}
		Logger::writeToLog(T("Music controller stopped"));
	}
	catch (Exception& ex)
	{
		// Exception is expected when thread is being aborted
		if(!threadShouldExit())
			Logger::writeToLog(T("Music controller exception: ") + ex.getFullMessage());
	}
}

// ** Handle messages posted to controller thread **
void MusicPlayer::handleMessage( const Message& message )
{
	// Delegate handling to private implementation
	_pimpl->handleMessage(message);
}

// ********************************
// *** Public interface methods ***
// ********************************

// Since some of the commands may take a while to process they
// are handled by a background thread to avoid locking the UI

void MusicPlayer::shutdown()
{
	_pimpl->_playerThread->stop();
	if(isThreadRunning())
		stopThread(1000);
}

void MusicPlayer::startPlaying()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::Play) );
}

void MusicPlayer::pausePlaying()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::Pause) );
}

void MusicPlayer::togglePlayPause()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::TogglePlayPause) );
}

void MusicPlayer::stopPlaying()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::Stop) );
}

void MusicPlayer::goToNext()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::Next) );
}

void MusicPlayer::goToPrevious()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::Previous) );
}

void MusicPlayer::refreshPlaylist()
{
	postThreadMessage( new PlayerMessage(PlayerMessage::RefreshPlaylist) );
}

bool MusicPlayer::performLogin( const String& login, const String& password )
{
	return _pimpl->performLogin(login, password);
}

bool MusicPlayer::isLoggedIn()
{
	return _pimpl->isLoggedIn();
}

Player::Status MusicPlayer::getPlayerStatus()
{
	return _pimpl->_playerThread->getPlayerStatus();
}

shared_ptr<SongInfo> MusicPlayer::getCurrentSong()
{
	return _pimpl->_playerThread->getCurrentSong();
}

uint32 MusicPlayer::getCurrentSongTime()
{
	return _pimpl->_playerThread->getCurrentSongTime();
}

void MusicPlayer::setCurrentSongTime( uint32 seconds )
{
	_pimpl->_playerThread->setCurrentSongTime(seconds);
}

void MusicPlayer::registerListener( ActionListener* const listener )
{
	_pimpl->_playerThread->addActionListener(listener);
}

