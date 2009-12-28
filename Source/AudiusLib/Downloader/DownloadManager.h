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

class DownloadBuffer;

class DownloadManager : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(DownloadManager, true)

private:
	DownloadManager(void);
	~DownloadManager(void);

public:
	boost::shared_ptr<DownloadBuffer> downloadAsync(const String& url);

	// Abort all current downloads
	void abortAll();

	// Shutdown download manager
	void shutdown();

	// Private methods
private:
	void cleanUp();

	// Private variables
private:
	//std::vector< boost::shared_ptr<DownloadThread> >	_downloadThreads;

};
