#include "Precompiled.h"
#include "Mp3AudioFormat.h"

//==============================================================================
#define mp3FormatName                          TRANS("Mp3 file")
static const tchar* const mp3Extensions[] =    { T(".mp3"), 0 };

Mp3AudioFormat::Mp3AudioFormat()
	: AudioFormat(mp3FormatName, (const tchar**) mp3Extensions)
{
}

Mp3AudioFormat::~Mp3AudioFormat()
{
}
