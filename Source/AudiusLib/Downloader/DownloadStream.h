#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: DownloadStream.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// A stream used when downloading data. The stream is filled in the background but can
// be accessed before it is completely filled.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "../System/Net/DataReceivedEventArgs.h"

class WebRequest;

class DownloadStream : public InputStream,
					   public ChangeBroadcaster
{
	// Only download manager can create download streams
	friend class DownloadManager;
private:
	DownloadStream(const String & url);

public:
	~DownloadStream(void);

	int64 getTotalLength() { return jmax(_bytesRead, _bytesTotal); }
	int64 getPosition() { return _readPosition; }

	// Returns true if the data is completely downloaded
	bool isDownloadComplete();

	// Returns true if the download is complete and we have read all of the data
	bool isExhausted();

	bool setPosition(int64 newPosition);
	int read(void* destBuffer, int maxBytesToRead);

	int64 getBytesAvailable() { return _bytesRead - _readPosition; }
	int64 getCurrentLength() { return _bytesRead; }
	int getEstimatedSecondsLeft() { return _secondsLeft; }

	// Get a raw pointer to the downloaded data
	void* getData() { return _buffer.getData(); }

	// Waits until the download is complete (-1 = infinite)
	bool wait(int timeoutMilliseconds);

	// Private methods called by the download manager to control the stream
private:
	// Starts downloading in the background
	void start();

	// Aborts the current download (done by manager)
	void abort();

	void receiveData(std::shared_ptr<DataReceivedEventArgs> args);
	void completed();

private:
	CriticalSection	_lock;

	std::shared_ptr<WebRequest> _request;

	MemoryBlock		_buffer;

	int64	_readPosition;
	int64	_bytesTotal;
	int64	_bytesRead;
	int64	_startTime;
	int		_secondsLeft;
};
