#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: SearchResult.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Holds a search result from a Spotify search operation.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "libspotify/include/libspotify/api.h"

class SearchResult
{
public:
	SearchResult(sp_search* search) :
	  _search(search)
	{
	}

	~SearchResult()
	{
	}

	void dispose()
	{
		if(_search)
		{
			sp_search_release(_search);
			_search = NULL;
		}
	}

private:
	sp_search*	_search;
};
