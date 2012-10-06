#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: HttpUtility.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains functionality commonly used in HTTP communication scenarios.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include <tchar.h>

class HttpUtility
{
private:
	HttpUtility() {}
	~HttpUtility() {}

public:

	/** Encodes a value for use in a url
	@param str	The string to encode
	*/
	static String urlEncode( const String& str )
	{
		String result;

		const int length = str.length();
		for (int i = 0; i < length; i++)
		{
			juce_wchar c = str[i];

			if(_istalnum(c))
				result += c;
			else
				if(_istspace(c))
					result += '+';
				else
				{
					int ic = _TINT(c);
					result += '%';
					result += toHex((byte)(ic>>4));
					result += toHex((byte)(ic%16));
				}
		}
		return result;
	}

private:
	/************************************************************************/
	/* URL encoding
	/************************************************************************/
	static inline TCHAR toHex(const BYTE &x)
	{
		return x > 9 ? _T('A') + x-10: _T('0') + x;
	}

};
