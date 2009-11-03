#include "Precompiled.h"
#include "DownloadThread.h"

#include "../System/Net/WebClient.h"
#include "../System/Net/WebClientFactory.h"
#include "../System/Net/DataReceivedEventArgs.h"

#include "../System/Exception.h"

using namespace boost;

// ******************************
// *** Private implementation ***
// ******************************
class DownloadThread::impl
{
private:
	impl(impl const &);
	impl& operator=(impl const &);

public:
	impl(const String& url, DownloadProgressDelegate callback) :
	  _downloadUrl(url),
	  _callback(callback)
	{
	}
	~impl()
	{
	}

	// Called for each chunk that is received
	void receiveData(shared_ptr<DataReceivedEventArgs> received)
	{
		if(_ownerThread->threadShouldExit())
		{
			received->cancelTransfer = true;
			Logger::writeToLog(T("Download aborted"));
			return;
		}

		// Calculate progress and setup event args
		shared_ptr<DownloadProgressEventArgs> progress( new DownloadProgressEventArgs() );

		int64 bytesTotal = received->getTotalBytes();
		int64 bytesReceived = received->getBytesReceived();
		_bytesRead += bytesReceived;

		//DBG_PRINTF((T("Bytes received: %lld (%lld / %lld)"), bytesReceived, _bytesRead, bytesTotal));

		// Done?
		if(_bytesRead >= bytesTotal)
		{
			progress->secondsLeft = 0;
			progress->isFinished = true;
		}
		else
		{
			// Estimate time left
			int64 ticksPassed = Time::currentTimeMillis() - _startTime;
			if(ticksPassed == 0)
				ticksPassed++;		// Avoid div by zero

			double bytesPerTick = _bytesRead / (double)ticksPassed;
			double ticksLeft = (bytesTotal - _bytesRead) / bytesPerTick;
			progress->secondsLeft = (uint32)ceil(ticksLeft / 1000);
			//DBG_PRINTF((T("Seconds left: %lds"), progress->secondsLeft));
		}

		progress->buffer = received->getData();
		progress->bufferSize = bytesReceived;
		progress->bytesRead = _bytesRead;
		progress->bytesTotal = bytesTotal;

		_callback(progress);
	}

	// Takes care of downloading
	void downloadData()
	{
		assert(_callback != NULL);

		DBG(T("Starting download from ") + _downloadUrl);

		_startTime = Time::currentTimeMillis();
		_bytesRead = 0;

		// Create web client and start downloading chunks
		shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
		DataReceivedDelegate callback = boost::bind(&impl::receiveData, this, _1);
		client->downloadChunks(_downloadUrl, callback);
	}

	const String& getUrl()
	{
		return _downloadUrl;
	}

public:
	DownloadThread*		_ownerThread;

	// Private variables
private:
	int64		_startTime;
	int64		_bytesRead;
	String		_downloadUrl;

	DownloadProgressDelegate	_callback;

};

// ******************************
// *** Constructor/destructor ***
// ******************************
DownloadThread::DownloadThread( const String& url, DownloadProgressDelegate callback ) :
	Thread(T("Downloader")),
	_pimpl( new impl(url, callback) )
{
	_pimpl->_ownerThread = this;
}

DownloadThread::~DownloadThread( void )
{
	delete _pimpl;
}

void DownloadThread::run()
{
	setPriority(0);

	try
	{
		// Delegate to private implementation
		_pimpl->downloadData();

		DBG(T("Download finished, url=") + _pimpl->getUrl());
	}
	catch (Exception& ex)
	{
		// Exception is expected when thread is being aborted
		if(!threadShouldExit())
			Logger::writeToLog(T("Download thread exception: ") + ex.getFullMessage());
	}
}