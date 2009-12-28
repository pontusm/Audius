#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebRequest.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Provides functionality for managing a single web request. Created through the
// WebRequestManager class.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "DataReceivedEventArgs.h"

struct WebRequestContext;

class WebRequest
{
public:
	WebRequest(const String & url);
	~WebRequest(void);

	// Abort an ongoing request
	void abort();

	// Begin downloading asynchronously
	void downloadAsync(DataReceivedDelegate callback);

	// Wait for the request to complete (-1 = infinite)
	bool wait(const int timeoutMilliseconds);

	const String & getUrl() { return url; }
	
	int getResponseCode();

private:
	// Generic callback for receiving data (had to make this static to get it working with Curl)
	static size_t receiveData(void *ptr, size_t size, size_t nmemb, void *customdata)
	{
		// Cast ptr to our object and pass control to its receive method
		WebRequest* pThis = (WebRequest*)customdata;
		return pThis->receiveDataInternal(ptr, size * nmemb);
	}

	size_t receiveDataInternal(void* ptr, uint32 receivedBytes);

	void handleError();

private:
	WebRequestContext*	context;

	String	url;
	int64	totalBytes;
};
