#include "Precompiled.h"
#include "WebRequestManager.h"

#include "WebException.h"

#include "WebRequestContext.h"

#include <curl/curl.h>

// *** Web request thread manages the currently active requests ***
class WebRequestThread : public Thread
{
public:
	WebRequestThread() :
		Thread(T("WebRequestThread")),
		multiHandle(NULL)
	{
	}
	~WebRequestThread()
	{
	}

	void run()
	{
		DBG(T("WebRequestThread started"))
		while(!threadShouldExit())
		{
			// Await pending request
			if(pendingRequest.wait(5000))
				processPendingRequests();
		}
		DBG(T("WebRequestThread shutdown"))
	}

	void signalPendingRequest()
	{
		pendingRequest.signal();
	}

public:
	CURLM* multiHandle;

private:
	// Keep processing requests until all are finished
	void processPendingRequests()
	{
		CURLMcode result;
		int transfersInProgress;
		while(!threadShouldExit())
		{
			// Process transfers
			result = curl_multi_perform(multiHandle, &transfersInProgress);
			if(result == CURLM_CALL_MULTI_PERFORM)
				continue;	// Need to call multi_perform again asap
			if(result != CURLM_OK)
				handleError(result);

			// All transfers finished?
			if(transfersInProgress == 0)
				break;

			//queryMessages();

			waitForSocketActivity();
		}
	}

	void waitForSocketActivity()
	{
		CURLMcode result;

		// Retrieve curl socket sets
		fd_set readset, writeset, exceptset;
		int max_fd;
		FD_ZERO(&readset); FD_ZERO(&writeset); FD_ZERO(&exceptset);
		result = curl_multi_fdset(multiHandle, &readset, &writeset, &exceptset, &max_fd);
		if(result != CURLE_OK)
			handleError(result);

		// No sockets active?
		if(max_fd == -1)
			return;

		// Retrieve max socket timeout (milliseconds)
		long timeout;
		result = curl_multi_timeout(multiHandle, &timeout);
		if(result != CURLE_OK)
			handleError(result);

		// Wait for socket activity
		timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout * 1000;		// Microseconds
		select(max_fd, &readset, &writeset, &exceptset, &tv);
	}

	void handleError(CURLMcode err)
	{
		String errmsg(curl_multi_strerror(err));
		throw WebException(errmsg);
	}

private:
	WaitableEvent pendingRequest;
};

// ********************
// *** Private data ***
// ********************
class WebRequestManager::impl
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

	WebRequestThread thread;

	//CURLM*	multiHandle;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
WebRequestManager::WebRequestManager(void) :
	vars( new impl() )
{
	// Init curl
	curl_global_init(CURL_GLOBAL_ALL);

	CURLM* multiHandle = curl_multi_init();
	if(!multiHandle)
		throw WebException(T("Failed to initialize libcurl."));

	// Start transfer tread
	vars->thread.multiHandle = multiHandle;
	vars->thread.startThread(0);
}

WebRequestManager::~WebRequestManager(void)
{
	// Shutdown transfer thread
	vars->thread.stopThread(5000);

	// Shutdown curl
	if(vars->thread.multiHandle)
		curl_multi_cleanup(vars->thread.multiHandle);
	curl_global_cleanup();

	delete vars;
	clearSingletonInstance();
}

// Singleton impl
juce_ImplementSingleton(WebRequestManager)
