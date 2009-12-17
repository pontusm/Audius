#include "Precompiled.h"
#include "WebClient.h"

#include "WebException.h"
#include "DataReceivedEventArgs.h"

#include <curl/curl.h>

#include <tchar.h>

using namespace boost;

// ******************************
// *** Private implementation ***
// ******************************
class WebClient::impl
{
private:
	impl(impl const &);
	impl& operator=(impl const &);

public:
	impl()
	{
	}
	~impl()
	{
	}

	CURL*	handle;
	char	errorBuffer[CURL_ERROR_SIZE];

	int64	totalBytes;

	DataReceivedDelegate	callback;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
WebClient::WebClient() :
	vars( new impl() )
{
	// Initialize curl session
	vars->handle = curl_easy_init();
	if(!vars->handle)
		throw WebException(T("Failed to initialize Curl."));

	// Setup error message buffer
	memset(vars->errorBuffer, 0, CURL_ERROR_SIZE);
	curl_easy_setopt(vars->handle, CURLOPT_ERRORBUFFER, vars->errorBuffer);
}

WebClient::~WebClient()
{
	if(vars->handle)
	{
		// Close curl session
		curl_easy_cleanup(vars->handle);
		vars->handle = NULL;
	}

	delete vars;
}

// ********************************
// *** Public interface methods ***
// ********************************

/** Download data from a URL and return it as a string */
String WebClient::downloadString(const String & url)
{
	String str;

	// Bind callback to our private impl class that will take care of filling the string
	//DataReceivedDelegate callback = boost::bind(&impl::downloadStringCallback, vars, &str, _1);
	//vars->downloadChunks(url, callback);

	return str;
}

void WebClient::downloadStream(const String & url, OutputStream & stream )
{
	//DataReceivedDelegate callback = boost::bind(&impl::downloadStreamCallback, vars, &stream, _1);
	//vars->downloadChunks(url, callback);
}

void WebClient::downloadChunks(const String & url, DataReceivedDelegate callback)
{
	//vars->downloadChunks(url, callback);
}

void WebClient::close()
{
	//vars->close();
}

/************************************************************************/
/* URL encoding
/************************************************************************/
inline TCHAR toHex(const BYTE &x)
{
	return x > 9 ? _T('A') + x-10: _T('0') + x;
}

String WebClient::urlEncode( const String& str )
{
	String result;

	const int length = str.length();
	for (int i = 0; i < length; i++)
	{
		tchar c = str[i];

		if(_istalnum(c))
			result += c;
		else
			if(_istspace(c))
				result += T('+');
			else
			{
				int ic = _TINT(c);
				result += T('%');
				result += toHex((byte)(ic>>4));
				result += toHex((byte)(ic%16));
			}
	}
	return result;
}
