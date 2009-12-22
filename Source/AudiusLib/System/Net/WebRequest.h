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
	//friend class WebRequestManager;
private:
	WebRequest(const String & url);

public:
	~WebRequest(void);

	static boost::shared_ptr<WebRequest> create(const String & url);

	void downloadAsync(DataReceivedDelegate callback);

	const String & getUrl() { return url; }

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

	String url;
};
