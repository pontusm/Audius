#include "Precompiled.h"
#include "DownloadStream.h"

#include "../System/Net/WebRequest.h"

using namespace boost;

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
}

void DownloadStream::start()
{
	_startTime = Time::currentTimeMillis();

	DataReceivedDelegate callback = boost::bind(&DownloadStream::receiveData, this, _1);
	_request->downloadAsync(callback);
}

void DownloadStream::abort()
{
	_request->abort();
}

void DownloadStream::receiveData( boost::shared_ptr<DataReceivedEventArgs> received )
{
	const ScopedLock l(_lock);

	int64 bufferOffset = _bytesRead;

	int64 bytesReceived = received->getBytesReceived();
	_bytesTotal = received->getTotalBytes();
	_bytesRead += bytesReceived;

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
