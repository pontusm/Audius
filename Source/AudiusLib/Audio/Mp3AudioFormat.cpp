#include "Precompiled.h"
#include "Mp3AudioFormat.h"
#include "../mpg123.h"

using namespace libmpg123Namespace;

//==============================================================================
#define mp3FormatName                          TRANS("Mp3 file")
static const tchar* const mp3Extensions[] =    { T(".mp3"), 0 };

class Mp3Reader : public AudioFormatReader
{
	mpg123_handle* _handle;
	AudioSampleBuffer _reservoir;
	int _reservoirStart, _samplesInReservoir;

public:
	Mp3Reader(InputStream* const inp)
		: AudioFormatReader(inp, mp3FormatName),
		_handle(NULL),
		_reservoir(2, 4096),
		_reservoirStart(0),
		_samplesInReservoir(0)
	{
		// Init base class members
		sampleRate = 0;
		usesFloatingPointData = true;

		//mpg123_init();
		_handle = mpg123_new(NULL, NULL);
		if(!_handle)
			Logger::writeToLog(T("Failed to init mp3 lib."));

		mpg123_open_feed(_handle);
	}

	~Mp3Reader()
	{
		mpg123_close(_handle);
		mpg123_delete(_handle);
	}

	bool readSamples(int** destSamples, int numDestChannels, int startOffsetInDestBuffer, int64 startSampleInFile, int numSamples)
	{
		while(numSamples > 0)
		{
			const int numAvailable = _reservoirStart + _samplesInReservoir - startSampleInFile;

			if (startSampleInFile >= _reservoirStart && numAvailable > 0)
			{
				// got a few samples overlapping, so use them before seeking..

				const int numToUse = jmin (numSamples, numAvailable);

				for (int i = jmin (numDestChannels, _reservoir.getNumChannels()); --i >= 0;)
					if (destSamples[i] != 0)
						memcpy (destSamples[i] + startOffsetInDestBuffer,
							_reservoir.getSampleData (i, (int) (startSampleInFile - _reservoirStart)),
							sizeof(float) * numToUse);

				startSampleInFile += numToUse;
				numSamples -= numToUse;
				startOffsetInDestBuffer += numToUse;

				if (numSamples == 0)
					break;
			}

			if (startSampleInFile < _reservoirStart
				|| startSampleInFile + numSamples > _reservoirStart + _samplesInReservoir)
			{
				// buffer miss, so refill the reservoir
				int bitStream = 0;

				_reservoirStart = jmax (0, (int) startSampleInFile);
				_samplesInReservoir = _reservoir.getNumSamples();

				off_t input_offset;
				if (_reservoirStart != mpg123_tell(_handle))
					mpg123_feedseek(_handle, _reservoirStart, SEEK_SET, &input_offset);

				int offset = 0;
				int numToRead = _samplesInReservoir;
/*
				while (numToRead > 0)
				{
					float** dataIn = 0;

					const int samps = ov_read_float (&ovFile, &dataIn, numToRead, &bitStream);
					if (samps == 0)
						break;

					jassert (samps <= numToRead);

					for (int i = jmin (numChannels, reservoir.getNumChannels()); --i >= 0;)
					{
						memcpy (reservoir.getSampleData (i, offset),
							dataIn[i],
							sizeof (float) * samps);
					}

					numToRead -= samps;
					offset += samps;
				}
*/
				if (numToRead > 0)
					_reservoir.clear(offset, numToRead);
			}

		}

		if (numSamples > 0)
		{
			for (int i = numDestChannels; --i >= 0;)
				if (destSamples[i] != 0)
					zeromem (destSamples[i] + startOffsetInDestBuffer, sizeof(int) * numSamples);
		}

		return true;
	}
};

Mp3AudioFormat::Mp3AudioFormat()
	: AudioFormat(mp3FormatName, (const tchar**) mp3Extensions)
{
}

Mp3AudioFormat::~Mp3AudioFormat()
{
}

const Array<int> Mp3AudioFormat::getPossibleSampleRates()
{
	const int rates[] = { 22050, 32000, 44100, 48000, 0 };
	return Array <int> (rates);
}

const Array<int> Mp3AudioFormat::getPossibleBitDepths()
{
	Array<int> depths;
	depths.add(32);
	return depths;
}

bool Mp3AudioFormat::canDoStereo()
{
	return true;
}

bool Mp3AudioFormat::canDoMono()
{
	return true;
}

AudioFormatReader* Mp3AudioFormat::createReaderFor(InputStream* sourceStream, const bool deleteStreamIfOpeningFails)
{
	Mp3Reader* r = new Mp3Reader(sourceStream);
	return r;
}

bool Mp3AudioFormat::isCompressed()
{
	return true;
}
