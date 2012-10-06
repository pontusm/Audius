#include "Precompiled.h"
#include "DownloadStream.h"

#include "../System/Net/WebRequest.h"

#include "DownloadManager.h"

using namespace std::placeholders;

DownloadStream::DownloadStream(const String & url) :
	_request( new WebRequest(url) ),
	_bytesRead(0),
	_bytesTotal(-1),
	_secondsLeft(-1),
	_readPosition(0)
{
}

DownloadStream::~DownloadStream(void)
{
	// Ensure it is no longer held by the download manager (will generate a call to abort() )
	DownloadManager* mgr = DownloadManager::getInstanceWithoutCreating();
	if(mgr)
		mgr->abort(this);
}

void DownloadStream::start()
{
	_startTime = Time::currentTimeMillis();

	DataReceivedDelegate receiveCallback = std::bind(&DownloadStream::receiveData, this, _1);
	RequestCompleteDelegate completeCallback = std::bind(&DownloadStream::completed, this);
	_request->downloadAsync(receiveCallback, completeCallback);
}

void DownloadStream::abort()
{
	_request->abort();
}

void DownloadStream::receiveData( std::shared_ptr<DataReceivedEventArgs> received )
{
	const ScopedLock l(_lock);

	int64 bufferOffset = _bytesRead;

	int64 bytesReceived = received->getBytesReceived();
	_bytesTotal = received->getTotalBytes();
	_bytesRead += bytesReceived;

	//DBG_PRINTF((T("Bytes received: %lld (%lld / %lld)"), bytesReceived, _bytesRead, _bytesTotal));

	// Need to resize buffer?
	int64 bufferSize = getTotalLength();
	_buffer.ensureSize((int)bufferSize);

	// Fill buffer
	_buffer.copyFrom(received->getData(), (int)bufferOffset, (int)bytesReceived);

	// Is total size known?
	if(_bytesTotal >= 0)
	{
		// Estimate time left
		int64 ticksPassed = Time::currentTimeMillis() - _startTime;
		if(ticksPassed == 0)
			ticksPassed++;		// Avoid div by zero

		double bytesPerTick = _bytesRead / (double)ticksPassed;
		double ticksLeft = (_bytesTotal - _bytesRead) / bytesPerTick;
		_secondsLeft = (int)ceil(ticksLeft / 1000);
	}

	// Notify listeners about progress
	sendChangeMessage();
}

void DownloadStream::completed()
{
	// Notify listeners that we are done
	sendChangeMessage();
}

bool DownloadStream::wait( int timeoutMilliseconds )
{
	return _request->wait(timeoutMilliseconds);
}

bool DownloadStream::isExhausted()
{
	const ScopedLock l(_lock);
	if(!_request->isCompleted())
		return false;
	return _readPosition >= getTotalLength();
}

bool DownloadStream::setPosition( int64 newPosition )
{
	const ScopedLock l(_lock);
	if(newPosition >= _bytesRead)
		return false;
	_readPosition = newPosition;
	return true;
}

int DownloadStream::read( void* destBuffer, int maxBytesToRead )
{
	const ScopedLock l(_lock);

	// Read from stream buffer
	int bytesAvailable = _bytesRead - _readPosition;
	int bytesToRead = jmin(bytesAvailable, maxBytesToRead);
	if(bytesToRead > 0)
	{
		_buffer.copyTo(destBuffer, _readPosition, bytesToRead);
		_readPosition += bytesToRead;
	}
	return bytesToRead;
}

bool DownloadStream::isDownloadComplete()
{
	const ScopedLock l(_lock);
	return _request->isCompleted();
}
