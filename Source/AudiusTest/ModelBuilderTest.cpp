#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/Model/ModelBuilder.h"
#include "AudiusLib/Model/SongInfo.h"

using namespace boost;

BEGIN_TEST(Model_ModelBuilder_CanCreateSongInfo)
{
	ModelBuilder builder;

	String xml( T("<?xml version=\"1.0\" encoding=\"utf-8\"?><s n=\"City on a Hill\" a=\"Genghis Tron\" l=\"5\" i=\"3671\" s=\"3972677\" c=\"0\" t=\"206\" />") );

	shared_ptr<SongInfo> songInfo = builder.createSongInfo(xml);
	WIN_ASSERT_TRUE(songInfo->getTitle().compare(T("City on a Hill")) == 0);
	WIN_ASSERT_TRUE(songInfo->getArtist().compare(T("Genghis Tron")) == 0);
	WIN_ASSERT_TRUE(songInfo->getSongID() == 3671);
	WIN_ASSERT_TRUE(songInfo->getSizeBytes() == 3972677);
	WIN_ASSERT_TRUE(songInfo->getLengthSeconds() == 206);

}
END_TEST
