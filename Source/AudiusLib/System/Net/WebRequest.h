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

// Complete delegate
typedef boost::function<void()> RequestCompleteDelegate;

struct WebRequestContext;

class WebRequest
{
public:
	WebRequest(const String & url);
	~WebRequest(void);

	// Abort an ongoing request
	void abort();

	// Begin downloading asynchronously
	void downloadAsync(DataReceivedDelegate receiveCallback, RequestCompleteDelegate completeCallback = NULL);

	// Begin posting asynchronously
	void postAsync(const StringPairArray & parameters, DataReceivedDelegate receiveCallback, RequestCompleteDelegate completeCallback = NULL);

	// Wait for the request to complete (-1 = infinite)
	bool wait(const int timeoutMilliseconds);

	const String & getUrl() { return url; }

	int getResponseCode();

	bool isStarted() { return started; }
	bool isCompleted() { return completed; }

	StringPairArray & getCookies() { return cookies; }
	void setCookies(StringPairArray & cookies_) { cookies = cookies_; }

private:
	// Generic callback for receiving data (had to make this static to get it working with Curl)
	static size_t receiveData(void *ptr, size_t size, size_t nmemb, void *customdata)
	{
		// Cast ptr to our object and pass control to its receive method
		WebRequest* pThis = (WebRequest*)customdata;
		return pThis->receiveDataInternal(ptr, size * nmemb);
	}

	size_t receiveDataInternal(void* ptr, int receivedBytes);

	void handleError();

	void setupRequest();

	// Methods called by the WebRequestController for signaling
private:
	friend class WebRequestController;
	void setComplete();

private:
	WebRequestContext*	context;

	String	url;
	int64	totalBytes;

	char*	postdataBuffer;
	char*	cookieBuffer;

	StringPairArray	cookies;

	volatile bool	started;
	volatile bool	completed;
	WaitableEvent	completeEvent;
};
