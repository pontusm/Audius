#include "Precompiled.h"
#include "ModelBuilder.h"

#include "SongInfo.h"

#include "../System/Exception.h"

using namespace boost;

ModelBuilder::ModelBuilder(void)
{
}

ModelBuilder::~ModelBuilder(void)
{
}

shared_ptr<SongInfo> ModelBuilder::createSongInfo(const String& xml )
{
	XmlDocument xd(xml);

	scoped_ptr<XmlElement> xe( xd.getDocumentElement() );
	if(xe == NULL || xe->getTagName() != T("s"))
	{
		//throw Exception(xd.getLastParseError());
		return shared_ptr<SongInfo>();		// No song info found
	}

	uint32 songID = xe->getIntAttribute(T("i"));
	uint32 sizeBytes = xe->getIntAttribute(T("s"));
	uint32 lengthSeconds = xe->getIntAttribute(T("t"));

	String title = xe->getStringAttribute(T("n"));
	String artist = xe->getStringAttribute(T("a"));

	return shared_ptr<SongInfo>( new SongInfo(songID, sizeBytes, lengthSeconds, title, artist) );
}