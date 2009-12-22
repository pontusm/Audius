#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: DataReceivedEventArgs.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains information about received data during downloads.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class DataReceivedEventArgs;

// Progress delegate
typedef boost::function<void (boost::shared_ptr<DataReceivedEventArgs> args)> DataReceivedDelegate;

class DataReceivedEventArgs
{
public:
	DataReceivedEventArgs(void* data, uint32 bytesReceived, int64 totalBytes) :
	  _data(data),
	  _bytesReceived(bytesReceived),
	  _totalBytes(totalBytes),
	  cancelTransfer(false)
	{
	}
	~DataReceivedEventArgs()
	{
	}

	const void* getData() const { return _data; }
	uint32 getBytesReceived() const { return _bytesReceived; }
	int64 getTotalBytes() const { return _totalBytes; }

public:
	// Set this flag to abort the transfer in progress
	bool	cancelTransfer;

private:
	void*	_data;
	uint32	_bytesReceived;
	int64	_totalBytes;
};
