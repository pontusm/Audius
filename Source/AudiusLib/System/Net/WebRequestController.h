#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebRequestController.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Internal class that manages all web requests that are currently in progress.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "WebRequest.h"

#include <curl/curl.h>

class WebRequestController : public Thread
{
public:
	WebRequestController() :
		Thread(T("WebRequestController")),
		multiHandle(NULL)
	{
	}
	~WebRequestController()
	{
	}

	void run()
	{
		DBG(T("WebRequestController started"))
		try
		{
			while(!threadShouldExit())
			{
				// Await pending request
				if(pendingRequest.wait(5000))
					processPendingRequests();
			}
		}
		catch (Exception& ex)
		{
			if(!threadShouldExit())
				Logger::writeToLog(T("Web request thread exception: ") + ex.getFullMessage());
		}
		DBG(T("WebRequestController shutdown"))
	}

	void addRequest(WebRequestContext* request)
	{
		CURLMcode result = curl_multi_add_handle(multiHandle, request->handle);
		if(result != CURLM_OK)
			handleError(result);

		lock.enter();
		requests.push_back(request);
		lock.exit();

		if(!isThreadRunning())
			startThread(0);

		pendingRequest.signal();
	}

	void removeRequest(WebRequestContext* request)
	{
		// Locate request and remove it from our list of known requests
		if( findRequest(request->handle, true) != NULL )
		{
			// Tell curl to cancel it
			CURLMcode result = curl_multi_remove_handle(multiHandle, request->handle);
			if(result != CURLM_OK)
				handleError(result);
		}
	}

public:
	CURLM* multiHandle;

private:
	// Keep processing requests until all are finished
	void processPendingRequests()
	{
		CURLMcode result;
		int transfersInProgress;
		int lastInProgress = -1;
		while(!threadShouldExit())
		{
			// Process transfers
			result = curl_multi_perform(multiHandle, &transfersInProgress);
			if(result == CURLM_CALL_MULTI_PERFORM)
				continue;	// Need to call multi_perform again asap
			if(result != CURLM_OK)
				handleError(result);

			// Any transfers finished?
			if(lastInProgress > transfersInProgress)
				queryMessages();

			lastInProgress = transfersInProgress;

			// All transfers finished?
			if(transfersInProgress == 0)
				break;

			waitForSocketActivity();
		}
	}

	void queryMessages()
	{
		int queueLength;
		CURLMsg* msg;
		while( (msg = curl_multi_info_read(multiHandle, &queueLength)) != NULL)
		{
			// Skip unknown messages
			if(msg->msg != CURLMSG_DONE)
				continue;

			// Notify request if known
			WebRequestContext* request = findRequest(msg->easy_handle, true);
			if(request)
			{
				// Signal request completed
				request->completed.signal();
			}
		}
	}

	// Searches the list of known requests and optionally removes it from the list
	WebRequestContext* findRequest(CURL* handle, bool removeRequest)
	{
		const ScopedLock l(lock);
		WebRequestContext* request = NULL;
		std::list<WebRequestContext*>::iterator iterator = requests.begin();
		while(iterator != requests.end())
		{
			if((*iterator)->handle == handle)
			{
				// Found it
				request = (*iterator);
				if(removeRequest)
					requests.erase(iterator);
				break;
			}
			++iterator;
		}
		return request;
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
	CriticalSection lock;
	WaitableEvent pendingRequest;

	std::list<WebRequestContext*> requests;
};
