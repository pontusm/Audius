#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: SearchOperation.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Takes care of a Spotify search operation.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "libspotify/include/libspotify/api.h"

#include "../SpotifyEventArgs.h"

class SearchOperation
{
public:
	SearchOperation(sp_session* session, const String& query, SpotifyEventDelegate callback) :
	  _search(NULL),
	  _session(session),
	  _query(query),
	  _eventCallback(callback)
	{
	}
			
	~SearchOperation(void)
	{
		dispose();
	}

	void dispose()
	{
		if(_search)
		{
			sp_search_release(_search);
			_search = NULL;
		}
	}

	void execute()
	{
		// TODO: Create search operation object (supply it with session+callback)
		_search = sp_search_create(_session,
									_query.toUTF8(),
									0, 10,
									0, 5,
									0, 3,
									&searchComplete, this);
	}

private:
	static void SP_CALLCONV searchComplete(sp_search* result, void* userdata)
	{
		SearchOperation* self = static_cast<SearchOperation*>(userdata);

		Log::write("Search returned total tracks: " + String(sp_search_total_tracks(result)));
	}

private:
	sp_session*	_session;
	sp_search*	_search;
	String		_query;

	SpotifyEventDelegate	_eventCallback;

};

