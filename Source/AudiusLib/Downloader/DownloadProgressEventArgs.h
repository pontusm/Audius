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

public:
	int		secondsLeft;
	bool	isFinished;

	int64	bytesTotal;
	int		bytesRead;
	int		bufferSize;

	// Pointer to the data buffer that is being filled
	const void*	buffer;
	//boost::shared_array<char>	buffer;
};

// Progress delegate
typedef std::function<void (std::shared_ptr<DownloadProgressEventArgs> args)> DownloadProgressDelegate;
