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
	void	downloadChunks(const String & url, DataReceivedDelegate callback);

	/** Encodes a value for use in a url
	@param str	The string to encode
	*/
	static String urlEncode(const String& str);

	int getTimeout() { return timeoutMilliseconds; }
	void setTimeout(int timeoutMilliseconds_) { timeoutMilliseconds = timeoutMilliseconds_; }

private:
	class impl;
	impl* pimpl;

	int		timeoutMilliseconds;
};
