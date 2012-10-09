#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: SongInfo.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains information about a song.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class SongInfo
{
public:
	SongInfo(int songID, int sizeBytes, int lengthSeconds, const String & title, const String & artist, const String & album) :
	  _songID(songID),
	  _sizeBytes(sizeBytes),
	  _lengthSeconds(lengthSeconds),
	  _title(title),
	  _artist(artist),
	  _album(album)
	{}
	~SongInfo(void)
	{}

	int getSongID() const { return _songID; }
	int getSizeBytes() const { return _sizeBytes; }
	int getLengthSeconds() const { return _lengthSeconds; }

	void setLengthSeconds(int seconds) { _lengthSeconds = seconds; }

	String getTitle() const { return _title; }
	String getArtist() const { return _artist; }
	String getAlbum() const { return _album; }

private:
	int	_songID;
	int	_sizeBytes;
	int	_lengthSeconds;

	String	_title;
	String	_artist;
	String	_album;
};
