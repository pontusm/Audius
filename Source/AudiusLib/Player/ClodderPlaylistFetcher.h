#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: ClodderPlaylistFetcher.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Fetches the current playlist from Clodder.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "../Services/ServiceManager.h"
#include "Playlist.h"

class ClodderPlaylistFetcher : public ThreadWithProgressWindow
{
public:
	ClodderPlaylistFetcher() :
		ThreadWithProgressWindow("Retrieving playlist...", true, true)
	{
		_clodder = ServiceManager::getInstance()->getClodder();
	}

	~ClodderPlaylistFetcher()
	{
	}

	void run()
	{
		std::shared_ptr<Playlist> playlist(new Playlist());

		double progress = 0;
		double step = 0.2;

		// Scan playlist from current position on server
		std::shared_ptr<PlaylistEntry> playlistEntry = getCurrentPlaylistEntryFromServer();
		while(playlistEntry)
		{
			progress += step;
			step *= 0.8;
			setProgress(progress);

			playlist->add(playlistEntry);

			if(threadShouldExit())
				break;

			playlistEntry = createPlaylistEntryFromServerXml( _clodder->gotoNext() );
		}

		setProgress(0.99);

		_playlist = playlist;

		// Step back to original position
		for(int i = 0; i < (int)playlist->getCount() - 1; i++)
			_clodder->gotoPrevious();

		setProgress(1);
	}

	// Retrieve the playlist
	std::shared_ptr<Playlist> getPlaylist() { return _playlist; }

private:
	// Retrieves the current song from server and creates a PlaylistEntry for it
	std::shared_ptr<PlaylistEntry> getCurrentPlaylistEntryFromServer()
	{
		String itemXml = _clodder->getCurrentPlaylistItem();
		return createPlaylistEntryFromServerXml(itemXml);
	}

	std::shared_ptr<PlaylistEntry> createPlaylistEntryFromServerXml(const String& itemXml)
	{
		std::shared_ptr<SongInfo> songInfo = _builder.createSongInfo(itemXml);
		if(!songInfo)
			return std::shared_ptr<PlaylistEntry>();

		String url = _clodder->getSongUrl(songInfo->getSongID());
		if(url.length() == 0)
			return std::shared_ptr<PlaylistEntry>();

		DBG(String("Scanned song: ") + songInfo->getArtist() + String(" - \"") + songInfo->getTitle() + String("\""));

		return std::shared_ptr<PlaylistEntry>( new PlaylistEntry(songInfo, url) );
	}


private:
	ModelBuilder _builder;

	std::shared_ptr<ClodderService> _clodder;
	std::shared_ptr<Playlist> _playlist;
};