#include "Precompiled.h"
#include "Mp3AudioFormat.h"
#include "../mpg123.h"

using namespace libmpg123Namespace;

//==============================================================================
#define mp3FormatName                          TRANS("Mp3 file")
static const tchar* const mp3Extensions[] =    { T(".mp3"), 0 };

class Mp3Reader : AudioFormatReader
{
	//mpg123_handle _handle;

public:
	Mp3Reader(InputStream* const inp)
		: AudioFormatReader(inp, mp3FormatName)
	{
		//mpg123_init();
	}

};
Mp3AudioFormat::Mp3AudioFormat()
	: AudioFormat(mp3FormatName, (const tchar**) mp3Extensions)
{
}

Mp3AudioFormat::~Mp3AudioFormat()
{
}
