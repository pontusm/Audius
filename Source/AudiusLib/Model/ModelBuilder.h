#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: ModelBuilder.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Creates object models from XML data.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "juce.h"

class SongInfo;

class ModelBuilder
{
public:
	ModelBuilder(void);
	~ModelBuilder(void);

	boost::shared_ptr<SongInfo> createSongInfo(const String& xml);

	std::vector< boost::shared_ptr<SongInfo> > createFromSearchResult(const String& searchResultHtml);
};
