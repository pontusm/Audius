#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: DownloadProgressEventArgs.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains information for a download in progress.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class DownloadProgressEventArgs
{
public:
	DownloadProgressEventArgs() :
	  bytesTotal(0),
	  bytesRead(0),
	  bufferSize(0),
	  secondsLeft(0),
	  isFinished(false)
	{
	}
	~DownloadProgressEventArgs()
	{
	}

	//bool getIsFinished() const { return _isFinished; }

	//int64 getBytesTotal() const { return _bytesTotal; }
	//int64 getBytesRead() const { return _bytesRead; }
	//int64 getBufferSize() const { return _bufferSize; }

	//uint32 getSecondsLeft() const { return _secondsLeft; }

	//const void* getBuffer() const { return _buffer; }

public:
	uint32	secondsLeft;
	bool	isFinished;

	uint64	bytesTotal;
	uint32	bytesRead;
	uint32	bufferSize;

	// Pointer to the data buffer that is being filled
	const void*	buffer;
	//boost::shared_array<char>	buffer;
};

// Progress delegate
typedef boost::function<void (boost::shared_ptr<DownloadProgressEventArgs> args)> DownloadProgressDelegate;
