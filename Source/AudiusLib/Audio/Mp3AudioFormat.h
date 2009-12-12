#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: Mp3AudioFormat.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Reads and writes audio in MP3 format.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class Mp3AudioFormat : public AudioFormat
{
public:
	Mp3AudioFormat(void);
	~Mp3AudioFormat(void);

	//==============================================================================
	const Array <int> getPossibleSampleRates();
	const Array <int> getPossibleBitDepths();
	bool canDoStereo();
	bool canDoMono();
	bool isCompressed();

	//==============================================================================
	AudioFormatReader* createReaderFor (InputStream* sourceStream,
		const bool deleteStreamIfOpeningFails);

	AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
		double sampleRateToUse,
		unsigned int numberOfChannels,
		int bitsPerSample,
		const StringPairArray& metadataValues,
		int qualityOptionIndex);

};
