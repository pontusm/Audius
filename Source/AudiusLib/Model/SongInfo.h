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
	SongInfo(uint32 songID, uint32 sizeBytes, uint32 lengthSeconds, const String & title, const String & artist) :
	  _songID(songID),
	  _sizeBytes(sizeBytes),
	  _lengthSeconds(lengthSeconds),
	  _title(title),
	  _artist(artist)
	{}
	~SongInfo(void)
	{}

	uint32 getSongID() const { return _songID; }
	uint32 getSizeBytes() const { return _sizeBytes; }
	uint32 getLengthSeconds() const { return _lengthSeconds; }

	String getTitle() const { return _title; }
	String getArtist() const { return _artist; }

private:
	uint32	_songID;
	uint32	_sizeBytes;
	uint32	_lengthSeconds;

	String	_title;
	String	_artist;
};
