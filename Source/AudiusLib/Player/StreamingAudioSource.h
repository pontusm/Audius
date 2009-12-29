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

class StreamingAudioSource : AudioSource
{
public:
	StreamingAudioSource(void);
	~StreamingAudioSource(void);

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void releaseResources();
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
};
