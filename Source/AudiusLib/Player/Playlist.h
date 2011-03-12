#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: Playlist.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// A thread safe list of songs to play. The music player has a reference to this list
// and the player thread uses it as well to know what to play.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "PlaylistEntry.h"

class Playlist
{
public:
	Playlist(void) :
	  _currentPosition(0)
	{
	}
	~Playlist(void)
	{
	}

	// Add an entry to the play list
	void add(std::shared_ptr<PlaylistEntry> entry)
	{
		const ScopedLock l(_lock);
		_entries.push_back(entry);
	}

	// Clears the entire play list
	void clear()
	{
		const ScopedLock l(_lock);
		_entries.clear();
		_currentPosition = 0;
	}

	// Retrieves the number of entries in the play list
	uint32 getCount()
	{
		const ScopedLock l(_lock);
		return _entries.size();
	}

	// Advances the counter to the next entry if possible, or returns false if at end of list
	bool gotoNextEntry()
	{
		const ScopedLock l(_lock);
		if(_currentPosition >= _entries.size() - 1)
			return false;
		_currentPosition++;
		return true;
	}

	bool gotoPreviousEntry()
	{
		const ScopedLock l(_lock);
		if(_currentPosition <= 0)
			return false;
		_currentPosition--;
		return true;
	}

	// Inserts the entry at a certain position in the play list
	void insert(uint32 position, std::shared_ptr<PlaylistEntry> entry)
	{
		const ScopedLock l(_lock);

		jassert(position >= 0 && position <= _entries.size());

		std::vector< std::shared_ptr<PlaylistEntry> >::iterator it = _entries.begin() + position;
		_entries.insert(it, entry);

		// Adjust current play position
		if(position <= _currentPosition)
			_currentPosition++;
	}

	// Gets the current play list entry
	std::shared_ptr<PlaylistEntry> getCurrentEntry()
	{
		const ScopedLock l(_lock);
		if(_currentPosition >= _entries.size())
			return std::shared_ptr<PlaylistEntry>();
		return _entries[_currentPosition];
	}

	// Gets the current play list position
	uint32 getCurrentPosition()
	{
		return _currentPosition;
	}

	// Sets the current position of the playlist
	bool setCurrentPosition(uint32 position)
	{
		const ScopedLock l(_lock);
		if(position < 0 || position >= _entries.size())
			return false;

		_currentPosition = position;
		return true;
	}

	std::shared_ptr<PlaylistEntry> getEntry(uint32 position)
	{
		const ScopedLock l(_lock);
		jassert(position >= 0 && position < _entries.size());
		return _entries[position];
	}

private:
	std::vector< std::shared_ptr<PlaylistEntry> >	_entries;

	uint32	_currentPosition;

	CriticalSection	_lock;

};
