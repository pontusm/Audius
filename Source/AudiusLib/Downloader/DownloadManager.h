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

class DownloadThread;

class DownloadManager : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(DownloadManager, true)

private:
	DownloadManager(void);
	~DownloadManager(void);

public:
	boost::shared_ptr<DownloadThread> downloadAsync(const String& url, DownloadProgressDelegate callback);

	// Abort all current downloads
	void abortAll();

	// Shutdown download manager
	void shutdown();

	// Private methods
private:
	void cleanUp();

	// Private variables
private:
	std::vector< boost::shared_ptr<DownloadThread> >	_downloadThreads;

};
