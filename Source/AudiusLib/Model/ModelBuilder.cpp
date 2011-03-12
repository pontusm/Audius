#include "Precompiled.h"
#include "ModelBuilder.h"

#include "SongInfo.h"

#include "../System/Exception.h"
#include "../System/Html/HtmlScraper.h"

ModelBuilder::ModelBuilder(void)
{
}

ModelBuilder::~ModelBuilder(void)
{
}

std::shared_ptr<SongInfo> ModelBuilder::createSongInfo(const String& xml )
{
	XmlDocument xd(xml);

	boost::scoped_ptr<XmlElement> xe( xd.getDocumentElement() );
	if(xe == NULL || xe->getTagName() != T("s"))
	{
		//throw Exception(xd.getLastParseError());
		return std::shared_ptr<SongInfo>();		// No song info found
	}

	int songID = xe->getIntAttribute(T("i"));
	int sizeBytes = xe->getIntAttribute(T("s"));
	int lengthSeconds = xe->getIntAttribute(T("t"));

	String title = xe->getStringAttribute(T("n"));
	String artist = xe->getStringAttribute(T("a"));

	return std::shared_ptr<SongInfo>( new SongInfo(songID, sizeBytes, lengthSeconds, title, artist, String::empty) );
}

std::vector< std::shared_ptr<SongInfo> > ModelBuilder::createFromSearchResult( const String& searchResultHtml )
{
	std::vector< std::shared_ptr<SongInfo> > songs;

	if(searchResultHtml.length() == 0)
		return songs;

	HtmlScraper scraper(searchResultHtml);
	while( scraper.findNextElement(T("tr"), T("class=\"ctxm_song\"")) )
	{
		String songID = scraper.getAttribute("songId");

		if(!scraper.findNextElement(T("a"), T("javascript:ps(")))
			continue;

		String title = scraper.getElementContents();

		if(!scraper.findNextElement(T("a"), T("href=\"#ar")))
			continue;

		String artist = scraper.getElementContents();

		if(!scraper.findNextElement(T("a"), T("href=\"#al")))
			continue;

		String album = scraper.getElementContents();

		std::shared_ptr<SongInfo> song( new SongInfo(songID.getIntValue(), -1, -1, title, artist, album) );
		songs.push_back(song);
	}

	return songs;
}
