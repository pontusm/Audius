#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

using namespace std::placeholders;

BEGIN_TEST(SystemNet_WebClient_CanDownloadString)
{
	WebClient client;
	try
	{
		String str = client.downloadString("http://www.google.com");
		WIN_ASSERT_TRUE(str.indexOfIgnoreCase("<html>") >= 0);
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg.toUTF16());
	}
}
END_TEST

BEGIN_TEST(SystemNet_WebClient_CanDownloadStream)
{
	WebClient client;
	try
	{
		MemoryOutputStream stream;
		client.downloadStream("http://www.google.com", stream);
		WIN_ASSERT_TRUE(stream.getDataSize() >= 0);
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg.toUTF16());
	}
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
void downloadAsync(std::shared_ptr<DataReceivedEventArgs> args)
{
	bytesReceived += args->getBytesReceived();
}

BEGIN_TEST(SystemNet_WebRequest_CanDownloadAsync)
{
	try
	{
		bytesReceived = 0;
		WebRequest request("http://www.google.com");
		DataReceivedDelegate callback = std::bind(downloadAsync, _1);
		request.downloadAsync(callback);
		WIN_ASSERT_TRUE( request.isStarted() );

		// Wait for request to complete
		//WIN_ASSERT_TRUE( request.wait(-1) );	// When debugging
		WIN_ASSERT_TRUE( request.wait(5000) );

		WIN_ASSERT_TRUE( request.getResponseCode() == 200 );
		WIN_ASSERT_TRUE( bytesReceived > 0 );
		WIN_ASSERT_TRUE( request.isCompleted() );
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg.toUTF16());
	}
}
END_TEST

void pausingCallback(std::shared_ptr<DataReceivedEventArgs> args)
{
	Thread::sleep(500);
}

BEGIN_TEST(SystemNet_WebRequest_CanAbortDownload)
{
	try
	{
		WebRequest request("http://www.google.com");
		DataReceivedDelegate callback = std::bind(pausingCallback, _1);
		request.downloadAsync(callback);
		request.abort();

		Thread::sleep(100);
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg.toUTF16());
	}
}
END_TEST

BEGIN_TEST(SystemNet_WebRequest_CanPostAsync)
{
	try
	{
		bytesReceived = 0;
		WebRequest request("http://www.google.com");
		DataReceivedDelegate callback = std::bind(downloadAsync, _1);
		StringPairArray params;
		params.set("q", "test");
		request.postAsync(params, callback);
		WIN_ASSERT_TRUE( request.isStarted() );
		WIN_ASSERT_TRUE( request.wait(5000) );
		WIN_ASSERT_TRUE( request.getResponseCode() == 405 );	// Post not allowed to Google
		WIN_ASSERT_TRUE( request.isCompleted() );
	}
	catch(Exception & ex)
	{
		String msg = ex.getFullMessage();
		WIN_ASSERT_FAIL(msg.toUTF16());
	}
}
END_TEST
