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
		// Initialize curl session
		_handle = curl_easy_init();
		if(!_handle)
			throw WebException(T("Failed to initialize Curl."));

		// Setup error message buffer
		memset(_errorBuffer, 0, CURL_ERROR_SIZE);
		curl_easy_setopt(_handle, CURLOPT_ERRORBUFFER, _errorBuffer);
	}
	~impl()
	{
		close();
	}

	void close()
	{
		if(_handle)
		{
			// Close curl session
			curl_easy_cleanup(_handle);
			_handle = NULL;
		}
	}

	// Generic callback for receiving data (had to make this static to get it working with Curl)
	static size_t receiveData(void *ptr, size_t size, size_t nmemb, void *customdata)
	{
		// Cast ptr to our object and pass control to its receive method
		impl* pThis = (impl*)customdata;
		return pThis->receiveDataInternal(ptr, size * nmemb);
	}

	// Downloads data using the default chunk size in curl (16KB)
	void downloadChunks(const String & url, DataReceivedDelegate callback)
	{
		assert(callback != NULL);

		_callback = callback;
		_totalBytes = -1;

		if( curl_easy_setopt(_handle, CURLOPT_URL, (const char*)url) != 0)
			handleError();

		// Setup the read callback
		if( curl_easy_setopt(_handle, CURLOPT_WRITEFUNCTION, receiveData) != 0)
			handleError();

		// Set the custom data to be passed to the callback
		if( curl_easy_setopt(_handle, CURLOPT_WRITEDATA, this) != 0)
			handleError();

		if( curl_easy_perform(_handle) != 0)
			handleError();
	}

	void downloadStringCallback(String * str, shared_ptr<DataReceivedEventArgs> args)
	{
		(*str) += String::fromUTF8((uint8*)args->getData(), args->getBytesReceived());
	}

	void downloadStreamCallback(OutputStream * stream, shared_ptr<DataReceivedEventArgs> args)
	{
		// Write data to output stream
		if(!stream->write(args->getData(), args->getBytesReceived()))
			args->cancelTransfer = true;
	}

private:
	// Generic receive data method (called by Curl)
	size_t receiveDataInternal(void* ptr, uint32 receivedBytes)
	{
		// Is total length not known?
		if(_totalBytes < 0)
		{
			// Determine content length
			double length = -1;
			if( curl_easy_getinfo(_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length) != CURLE_OK )
				handleError();
			_totalBytes = (int64)length;
		}

		// Pass it on to the desired callback method
		shared_ptr<DataReceivedEventArgs> args( new DataReceivedEventArgs( ptr, receivedBytes, _totalBytes ) );
		_callback(args);

		// If callback wants to cancel transfer we return 0 to curl to make it stop
		if(args->cancelTransfer)
			return 0;

		// Return bytes handled to make curl continue
		return receivedBytes;
	}

	void handleError()
	{
		// Wrap up the error message and throw an exception with the error
		String errmsg(_errorBuffer);
		throw WebException(errmsg);
	}

private:
	CURL*	_handle;
	char	_errorBuffer[CURL_ERROR_SIZE];

	int64	_totalBytes;

	DataReceivedDelegate	_callback;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
WebClient::WebClient() :
	_pimpl( new impl() )
{
}

WebClient::~WebClient()
{
	delete _pimpl;
}

// ********************************
// *** Public interface methods ***
// ********************************

/** Download data from a URL and return it as a string */
String WebClient::downloadString(const String & url)
{
	String str;

	// Bind callback to our private impl class that will take care of filling the string
	DataReceivedDelegate callback = boost::bind(&impl::downloadStringCallback, _pimpl, &str, _1);
	_pimpl->downloadChunks(url, callback);

	return str;
}

void WebClient::downloadStream(const String & url, OutputStream & stream )
{
	DataReceivedDelegate callback = boost::bind(&impl::downloadStreamCallback, _pimpl, &stream, _1);
	_pimpl->downloadChunks(url, callback);
}

void WebClient::downloadChunks(const String & url, DataReceivedDelegate callback)
{
	_pimpl->downloadChunks(url, callback);
}

void WebClient::close()
{
	_pimpl->close();
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
				result += toHex((BYTE)(ic>>4));
				result += toHex((BYTE)(ic%16));
			}
	}
	return result;
}
