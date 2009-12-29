#include "Precompiled.h"
#include "DownloadStream.h"

#include "../System/Net/WebRequest.h"

using namespace boost;

DownloadStream::DownloadStream(const String & url) :
	_request( new WebRequest(url) ),
	_bytesRead(0),
	_secondsLeft(-1)
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

	int64 bytesTotal = received->getTotalBytes();
	int64 bytesReceived = received->getBytesReceived();
	_bytesRead += bytesReceived;

	// Need to resize buffer?
	int bufferSize = jmax(bytesTotal, _bytesRead);
	_buffer.ensureSize(bufferSize);

	// Fill buffer
	_buffer.copyFrom(received->getData(), bufferOffset, bytesReceived);

	// Is total size known?
	if(bytesTotal >= 0)
	{
		// Estimate time left
		int64 ticksPassed = Time::currentTimeMillis() - _startTime;
		if(ticksPassed == 0)
			ticksPassed++;		// Avoid div by zero

		double bytesPerTick = _bytesRead / (double)ticksPassed;
		double ticksLeft = (bytesTotal - _bytesRead) / bytesPerTick;
		_secondsLeft = (int)ceil(ticksLeft / 1000);
	}

}

bool DownloadStream::wait( int timeoutMilliseconds )
{
	return _request->wait(timeoutMilliseconds);
}
