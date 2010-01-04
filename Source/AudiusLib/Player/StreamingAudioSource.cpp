#include "Precompiled.h"
#include "StreamingAudioSource.h"

#include "../Downloader/DownloadManager.h"
#include "../Downloader/DownloadStream.h"

StreamingAudioSource::StreamingAudioSource(const String & url, AudioFormat & format) :
	_buffering(false)
{
	// Start download. The stream is freed by the AudioFormatReader.
	_stream = DownloadManager::getInstance()->downloadAsync(url);

	// Decoder requires some data to be present so we wait until we have some
	while(!_stream->isExhausted() && _stream->getBytesAvailable() < 1024)
		Thread::sleep(100);

	// Create decoder
	AudioFormatReader* reader = format.createReaderFor(_stream, false);
	_readerSource = new AudioFormatReaderSource(reader, true);
}

StreamingAudioSource::~StreamingAudioSource(void)
{
	// This also deletes the stream
	deleteAndZero(_readerSource);
}

void StreamingAudioSource::prepareToPlay( int samplesPerBlockExpected, double sampleRate )
{
	_readerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
	//_stream->setPosition(0);
}

void StreamingAudioSource::releaseResources()
{
	_readerSource->releaseResources();
}

void StreamingAudioSource::getNextAudioBlock( const AudioSourceChannelInfo& info )
{
	bool needBuffering = false;

	// Run out of data? (TODO: Need to calculate bytes/sample here)
	if(!_stream->isExhausted() && _stream->getBytesAvailable() == 0)
		needBuffering = true;
	//else
	//{
	//	// Check if the stream has filled up yet
	//	int secondsLeft = _stream->getEstimatedSecondsLeft();
	//	if(secondsLeft >= 0 && secondsLeft <= _totalSeconds)
	//		needBuffering = false;
	//	else
	//		needBuffering = true;
	//}

	if(!needBuffering)
	{
		// Output audio from decoder
		_readerSource->getNextAudioBlock(info);

		// Were we just buffering?
		if(_buffering)
		{
			// Fade in
			for (int i = info.buffer->getNumChannels(); --i >= 0;)
			{
				info.buffer->applyGainRamp(i, info.startSample, jmin(256, info.numSamples), 0, 1.0f);
			}

		}
	}
	else
	{
		// Just output silence
		info.clearActiveBufferRegion();
	}

	_buffering = needBuffering;
}

void StreamingAudioSource::setNextReadPosition( int newPosition )
{
	_readerSource->setNextReadPosition(newPosition);
}

int StreamingAudioSource::getNextReadPosition() const
{
	return _readerSource->getNextReadPosition();
}

int StreamingAudioSource::getTotalLength() const
{
	// A bit of a hack to correctly stop the source once it has run out of data
	if(_stream->isExhausted() && _stream->getBytesAvailable() == 0)
		return _readerSource->getNextReadPosition() - 2;

	// This forces audio to go on until entire stream is exhausted
	// Had to use this because estimated mp3 length was incorrect sometimes
	return _readerSource->getNextReadPosition();
	//return _readerSource->getTotalLength();
}
