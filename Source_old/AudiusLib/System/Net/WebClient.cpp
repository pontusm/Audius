#include "Precompiled.h"
#include "WebClient.h"

#include "WebException.h"
#include "WebRequest.h"
#include "DataReceivedEventArgs.h"

#include <curl/curl.h>

using namespace std::placeholders;

// ******************************
// *** Private implementation ***
// ******************************
class WebClient::impl
{
private:
	impl(impl const &);
	impl& operator=(impl const &);

public:
	impl()
	{
	}
	~impl()
	{
	}

	void downloadStringCallback(String * str, std::shared_ptr<DataReceivedEventArgs> args)
	{
		(*str) += String::fromUTF8((const char*)args->getData(), args->getBytesReceived());
	}

	void downloadStreamCallback(OutputStream * stream, std::shared_ptr<DataReceivedEventArgs> args)
	{
		// Write data to output stream
		if(!stream->write(args->getData(), args->getBytesReceived()))
			args->cancelTransfer = true;
	}

public:
	std::shared_ptr<WebRequest> request;

private:
	DataReceivedDelegate	_callback;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
WebClient::WebClient() :
	pimpl( new impl() ),
	timeoutMilliseconds(10 * 1000)
{
}

WebClient::~WebClient()
{
	delete pimpl;
}

// ********************************
// *** Public interface methods ***
// ********************************

/** Download data from a URL and return it as a string */
String WebClient::downloadString(const String & url)
{
	String str;
	WebRequest request(url);

	// Bind callback to our private impl class that will take care of filling the string
	DataReceivedDelegate callback = std::bind(&impl::downloadStringCallback, pimpl, &str, _1);
	request.downloadAsync(callback);
	if(!request.wait(timeoutMilliseconds))
		throw WebException("Operation timed out");

	return str;
}

//void WebClient::downloadChunks(const String & url, DataReceivedDelegate callback)
//{
//	//_pimpl->downloadChunks(url, callback);
//}

void WebClient::downloadStream(const String & url, OutputStream & stream )
{
	WebRequest request(url);

	DataReceivedDelegate callback = std::bind(&impl::downloadStreamCallback, pimpl, &stream, _1);
	request.downloadAsync(callback);
	if(!request.wait(timeoutMilliseconds))
		throw WebException("Operation timed out");
}

String WebClient::post( const String & url, const StringPairArray & parameters )
{
	String str;
	WebRequest request(url);
	request.setCookies(cookies);

	// Bind callback to our private impl class that will take care of filling the string
	DataReceivedDelegate callback = std::bind(&impl::downloadStringCallback, pimpl, &str, _1);
	request.postAsync(parameters, callback);
	if(!request.wait(timeoutMilliseconds))
		throw WebException("Operation timed out");

	return str;
}
