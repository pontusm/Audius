#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

BEGIN_TEST(Downloader_CanDownloadStream)
{
	try
	{
		// Cannot test this right now since it relies on MessageManager...
		//DownloadStream* stream = DownloadManager::getInstance()->downloadAsync(T("http://www.google.com"));
		//stream->wait(5000);

		//WIN_ASSERT_TRUE( stream->getCurrentLength() >= 0 );

		//delete stream;
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg.toUTF16());
	}
}
END_TEST
