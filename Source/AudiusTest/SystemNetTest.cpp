#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

using namespace boost;

BEGIN_TEST(SystemNet_WebClient_CanDownloadString)
{
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	try
	{
		String str = client->downloadString(T("http://www.google.com"));
		WIN_ASSERT_TRUE(str.indexOfIgnoreCase(T("<html>")) >= 0);
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg);
	}
	client->close();
}
END_TEST

void ReceiveData(shared_ptr<DataReceivedEventArgs> args)
{
	WIN_ASSERT_TRUE(args->getBytesReceived() > 0);
}

BEGIN_TEST(SystemNet_WebRequest_CanDownloadChunks)
{
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();

	try
	{
		DataReceivedDelegate callback = boost::bind(ReceiveData, _1);
		client->downloadChunks(T("http://www.google.com"), callback);
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg);
	}
	client->close();
}
END_TEST
