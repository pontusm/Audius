#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebClient.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains methods for performing web client operations such as HTTP transfers.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "DataReceivedEventArgs.h"

class WebClient
{
public:
	WebClient();
	~WebClient();

	/** Loads content from a url and returns it as a string */
	String	downloadString(const String & url);
	void	downloadStream(const String & url, OutputStream & stream);
	//void	downloadChunks(const String & url, DataReceivedDelegate callback);

	// Post to an url and return the response as a string
	String	post(const String & url, const StringPairArray & parameters);

	int getTimeout() { return timeoutMilliseconds; }
	void setTimeout(int timeoutMilliseconds_) { timeoutMilliseconds = timeoutMilliseconds_; }

	void setCookies(const StringPairArray & cookies_) { cookies = cookies_; }

private:
	class impl;
	impl* pimpl;

	int		timeoutMilliseconds;

	StringPairArray cookies;
};
