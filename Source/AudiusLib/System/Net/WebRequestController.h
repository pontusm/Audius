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

	void signalPendingRequest()
	{
		if(!isThreadRunning())
			startThread(0);

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
			// TODO: Signal request completed
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
