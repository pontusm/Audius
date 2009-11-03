#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: PlaylistEntry.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains information about a single entry in a playlist.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class SongInfo;

class PlaylistEntry
{
public:
	PlaylistEntry(boost::shared_ptr<SongInfo> songInfo, const String& url) :
	  _songInfo(songInfo),
	  _url(url)
	{
	}
	~PlaylistEntry(void)
	{}

	boost::shared_ptr<SongInfo> getSongInfo()
	{
		return _songInfo;
	}

	const String& getUrl()
	{
		return _url;
	}

private:
	String	_url;

	boost::shared_ptr<SongInfo>	_songInfo;

};
