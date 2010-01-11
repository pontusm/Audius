#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: StreamingAudioSource.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// An audio source that plays music from a streaming buffer.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class DownloadStream;

class StreamingAudioSource : public PositionableAudioSource
{
public:
	StreamingAudioSource(const String & url, AudioFormat & format);
	~StreamingAudioSource(void);

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void releaseResources();
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

	int getNextReadPosition() const;
	int getTotalLength() const;
	void setNextReadPosition(int newPosition);
	bool isLooping() const { return false; }

	// Retrieves the current download stream. Do not delete it!
	DownloadStream* getStream() { return _stream; }

private:
	DownloadStream* _stream;

	AudioFormatReaderSource* _readerSource;

	bool _buffering;
};
