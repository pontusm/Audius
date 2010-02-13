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
	byte _samplebuffer[4096 * 2 * 2];
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
		bitsPerSample = 16;
		usesFloatingPointData = true;

		_handle = mpg123_new(NULL, NULL);
		if(!_handle)
			Log::write(T("Failed to init mp3 lib."));

		mpg123_replace_reader(_handle, &mp3ReadCallback, &mp3SeekCallback);
		//mpg123_open_feed(_handle);
		mpg123_open_fd(_handle, (int)inp);
		
		// Scan until format can be determined
		//int64 startpos = inp->getPosition();
		//byte buffer[1024];
		int result = MPG123_OK;
		while(result != MPG123_ERR && result != MPG123_DONE && !inp->isExhausted())
		{
			off_t frameNum;
			byte* audiobuffer;
			size_t bytesread;
			result = mpg123_decode_frame(_handle, &frameNum, &audiobuffer, &bytesread);
			if(result == MPG123_NEW_FORMAT)
			{
				long rate;
				int channels;
				int encoding;
				result = mpg123_getformat(_handle, &rate, &channels, &encoding);
				if(result == MPG123_OK)
				{
					sampleRate = rate;
					numChannels = channels;

					if(encoding & MPG123_ENC_SIGNED_16)
						bitsPerSample = 16;
					else
						Log::write(T("Unknown encoding: ") + String(encoding));

					break;
				}
			}
		}

		//inp->setPosition(startpos);		// Reset stream

		// Configure decoder
		//if(sampleRate > 0)
		//{
		//	result = mpg123_format(_handle, 44100, MPG123_STEREO, MPG123_ENC_FLOAT_32);
		//	if(result != MPG123_OK)
		//		Log::write(T("Failed to set mp3 output format."));
		//}
		result = mpg123_scan(_handle);
		if(result == MPG123_OK)
		{
			int64 totalBytes = input->getTotalLength();
			if(totalBytes > 0)
				mpg123_set_filesize(_handle, (off_t)totalBytes);
			lengthInSamples = mpg123_length(_handle);
		}
		else
			lengthInSamples = -1;

		//DBG(T("Length in samples: ") + String(lengthInSamples));
	}

	~Mp3Reader()
	{
		mpg123_close(_handle);
		mpg123_delete(_handle);
	}

	inline float fixedToIEEEfloat(const short val) const
	{
		const double factor = 1.0/0x7fff;
		return (float)jlimit(-1.0, 1.0, (val * factor));
	} 

	bool readSamples(int** destSamples, int numDestChannels, int startOffsetInDestBuffer, int64 startSampleInFile, int numSamples)
	{
		while(numSamples > 0)
		{
			const int numAvailable = _reservoirStart + _samplesInReservoir - (int)startSampleInFile;

			if (startSampleInFile >= _reservoirStart && numAvailable > 0)
			{
				// got a few samples overlapping, so use them before seeking..

				const int numToUse = jmin (numSamples, numAvailable);

				for (int i = jmin (numDestChannels, _reservoir.getNumChannels()); --i >= 0;)
					if (destSamples[i] != 0)
						memcpy(destSamples[i] + startOffsetInDestBuffer,
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

				if (_reservoirStart != mpg123_tell(_handle))
				{
					off_t input_offset;
					off_t result = mpg123_feedseek(_handle, _reservoirStart, SEEK_SET, &input_offset);
					//if(result < 0)
					//	Log::write(T("Failed to seek mp3 stream. Error code: " + String(result) ));
					input->setPosition(input_offset);
				}

				int offset = 0;
				int numToRead = _samplesInReservoir;

				while(numToRead > 0)
				{
					size_t bytesread;
					int result = mpg123_read(_handle, _samplebuffer, sizeof(_samplebuffer), &bytesread);

					if(bytesread == 0 || result != MPG123_OK)
						break;

					const int samps = bytesread / sizeof(short) / numChannels;
					jassert(samps <= numToRead);

					// Fill reservoir (TODO: Optimize this)
					for (int i = jmin ((int)numChannels, (int)_reservoir.getNumChannels()); --i >= 0;)
					{
						float* buffer = _reservoir.getSampleData(i, offset);
						short* src = ((short*)_samplebuffer) + i;
						for(int j = 0; j < samps; j++)
						{
							(*buffer++) = fixedToIEEEfloat((*src));
							src += numChannels;		// Skip other channel data
						}
					}

					numToRead -= samps;
					offset += samps;
				}

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

		// Extract extra mp3 info
		mpg123_frameinfo frameInfo;
		int result = mpg123_info(_handle, &frameInfo);
		if(result == MPG123_OK)
		{
			metadataValues.set(T("samplerate"), String(frameInfo.rate));
			metadataValues.set(T("bitrate"), String(frameInfo.bitrate));

			//lengthInSamples = mpg123_length(_handle);
		}

		return true;
	}

	static ssize_t mp3ReadCallback(int fd, void* buffer, size_t bytesToRead)
	{
		return ((InputStream*)fd)->read(buffer, bytesToRead);
	}

	static off_t mp3SeekCallback(int fd, off_t offset, int whence)
	{
		InputStream* const input = (InputStream*)fd;
		if (whence == SEEK_CUR)
			offset += (off_t)input->getPosition();
		else if (whence == SEEK_END)
			offset += (off_t)input->getTotalLength();

		if(!input->setPosition(offset))
			return -1;
		return (off_t)input->getPosition();
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

bool Mp3AudioFormat::isCompressed()
{
	return true;
}

AudioFormatReader* Mp3AudioFormat::createReaderFor(InputStream* sourceStream, const bool deleteStreamIfOpeningFails)
{
	Mp3Reader* r = new Mp3Reader(sourceStream);
	if (r->sampleRate == 0)
		if (deleteStreamIfOpeningFails)
			delete r;
	return r;
}

AudioFormatWriter* Mp3AudioFormat::createWriterFor( OutputStream* streamToWriteTo, double sampleRateToUse, unsigned int numberOfChannels, int bitsPerSample, const StringPairArray& metadataValues, int qualityOptionIndex )
{
	return NULL;
}
