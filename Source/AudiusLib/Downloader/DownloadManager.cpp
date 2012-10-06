#include "Precompiled.h"
#include "DownloadManager.h"

#include "DownloadStream.h"
#include "DownloadProgressEventArgs.h"

DownloadManager::DownloadManager()
{
}

DownloadManager::~DownloadManager()
{
	clearSingletonInstance();
}

DownloadStream* DownloadManager::downloadAsync(const String& url)
{
	// Create stream and start downloading
	DownloadStream* stream = new DownloadStream(url);
	_downloadStreams.push_back(stream);
	stream->start();
	return stream;
}

void DownloadManager::abortAll()
{
	// Abort all downloads
	std::list< DownloadStream* >::iterator iterator = _downloadStreams.begin();
	while(iterator != _downloadStreams.end())
	{
		(*iterator)->abort();
		iterator = _downloadStreams.erase(iterator);
	}
}

void DownloadManager::abort( DownloadStream* stream )
{
	stream->abort();
	_downloadStreams.remove(stream);
}

void DownloadManager::shutdown()
{
	Log::write("Download manager closing");
	if(_downloadStreams.size() > 0)
		abortAll();
}

// Singleton impl
juce_ImplementSingleton(DownloadManager)
