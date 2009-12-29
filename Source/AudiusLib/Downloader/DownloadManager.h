#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: DownloadManager.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles all ongoing downloads.
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
	boost::shared_ptr<DownloadStream> downloadAsync(const String& url);

	// Cancels the specified download
	void abort(boost::shared_ptr<DownloadStream> stream);

	// Abort all current downloads
	void abortAll();

	// Shutdown download manager
	void shutdown();

	// Private methods
private:

	// Private variables
private:
	std::list< boost::shared_ptr<DownloadStream> >	_downloadStreams;

};
