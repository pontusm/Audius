#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

using namespace boost;

BEGIN_TEST(SystemNet_WebClient_CanDownloadString)
{
	//shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	//try
	//{
	//	String str = client->downloadString(T("http://www.google.com"));
	//	WIN_ASSERT_TRUE(str.indexOfIgnoreCase(T("<html>")) >= 0);
	//}
	//catch(Exception & ex)
	//{
	//	String msg = ex.getFullMessage();
	//	WIN_ASSERT_FAIL(msg);
	//}
	//client->close();
}
END_TEST

BEGIN_TEST(SystemNet_WebClient_CanDownloadStream)
{
	//shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	//try
	//{
	//	MemoryOutputStream stream;
	//	client->downloadStream(T("http://www.google.com"), stream);
	//	WIN_ASSERT_TRUE(stream.getDataSize() >= 0);
	//}
	//catch(Exception & ex)
	//{
	//	String msg = ex.getFullMessage();
	//	WIN_ASSERT_FAIL(msg);
	//}
	//client->close();
}
END_TEST

BEGIN_TEST(SystemNet_WebRequest_CanDownloadChunks)
{
	//WebRequestManager::getInstance()->beginDownload();
	//shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();

	//try
	//{
	//	DataReceivedDelegate callback = boost::bind(ReceiveData, _1);
	//	client->downloadChunks(T("http://www.google.com"), callback);
	//}
	//catch(Exception & ex)
	//{
	//	String msg = ex.getFullMessage();
	//	WIN_ASSERT_FAIL(msg);
	//}
	//client->close();
}
END_TEST

int bytesReceived = 0;
void downloadAsync(shared_ptr<DataReceivedEventArgs> args)
{
	bytesReceived += args->getBytesReceived();
}

BEGIN_TEST(SystemNet_WebRequest_CanDownloadAsync)
{
	try
	{
		shared_ptr<WebRequest> request = WebRequest::create(T("http://www.google.com"));
		DataReceivedDelegate callback = boost::bind(downloadAsync, _1);
		request->downloadAsync(callback);

		// Wait for request to complete
		//WIN_ASSERT_TRUE( request->wait(-1) );	// When debugging
		WIN_ASSERT_TRUE( request->wait(5000) );

		WIN_ASSERT_TRUE( request->getResponseCode() == 200 );
		WIN_ASSERT_TRUE( bytesReceived > 0 );
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg);
	}
}
END_TEST
