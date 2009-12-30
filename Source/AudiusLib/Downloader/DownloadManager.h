#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: DownloadManager.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles all ongoing downloads. Raw pointers are used for the streams to work better
// with Juce since AudioFormatReader wants to delete pointers.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "DownloadProgressEventArgs.h"

class DownloadStream;

class DownloadManager : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(DownloadManager, true)

private:
	DownloadManager(void);
	~DownloadManager(void);

public:
	// Starts a new download. The caller is responsible for freeing the stream.
	DownloadStream* downloadAsync(const String& url);

	// Cancels the specified download
	void abort(DownloadStream* stream);

	// Abort all current downloads
	void abortAll();

	// Shutdown download manager
	void shutdown();

	// Private methods
private:

	// Private variables
private:
	std::list< DownloadStream* >	_downloadStreams;

};
