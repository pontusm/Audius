#include "Precompiled.h"
#include "DownloadManager.h"

#include "DownloadThread.h"
#include "DownloadProgressEventArgs.h"

using namespace std;
using namespace boost;

DownloadManager::DownloadManager(void)
{
}

DownloadManager::~DownloadManager(void)
{
}

shared_ptr<DownloadThread> DownloadManager::downloadAsync(const String& url, DownloadProgressDelegate callback)
{
	// Create new thread and add it to thread list
	shared_ptr<DownloadThread> t( new DownloadThread(url, callback) );
	_downloadThreads.push_back(t);
	t->startThread();
	return t;
}

void DownloadManager::cleanUp()
{
	// Clean up threads that have stopped
	vector< shared_ptr<DownloadThread> >::iterator iterator = _downloadThreads.begin();
	while(iterator != _downloadThreads.end())
	{
		if(!(*iterator)->isThreadRunning())
			iterator = _downloadThreads.erase(iterator);
		else
			++iterator;
	}
	
}

void DownloadManager::abortAll()
{
	// Stop all threads
	/*
	vector< shared_ptr<DownloadThread> >::iterator iterator;
	for(iterator = _downloadThreads.begin(); iterator != _downloadThreads.end(); iterator++)
	{
		if((*iterator)->isThreadRunning())
		{
			Logger::writeToLog(T("Aborting thread: ") + (*iterator)->getThreadName());
			Sleep(5000);
			(*iterator)->stopThread(10000);
		}
	}
	*/
	Thread::stopAllThreads(10000);
	cleanUp();
}

void DownloadManager::shutdown()
{
	Logger::writeToLog(T("Download manager closing"));
	abortAll();
}

// Singleton impl
juce_ImplementSingleton(DownloadManager)
