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

class DataReceivedEventArgs;

// Progress delegate
typedef boost::function<void (boost::shared_ptr<DataReceivedEventArgs> args)> DataReceivedDelegate;

class WebClient
{
	// Only allow singleton factory to create instances
	friend class WebClientFactory;
private:
	WebClient();

public:
	~WebClient();

	/** Loads content from a url and returns it as a string */
	String	downloadString(const String & url);
	void	downloadStream(const String & url, OutputStream & stream);
	void	downloadChunks(const String & url, DataReceivedDelegate callback);

	void	close();

	/** Encodes a value for use in a url
	@param str	The string to encode
	*/
	static String urlEncode(const String& str);

private:
	class impl;
	impl* _pimpl;
};
