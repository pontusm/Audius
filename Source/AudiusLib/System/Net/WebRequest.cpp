#include "Precompiled.h"
#include "WebRequest.h"

#include "WebRequestContext.h"
#include "WebRequestManager.h"
#include "WebException.h"

using namespace boost;

WebRequest::WebRequest(const String & url) :
	url(url),
	totalBytes(-1)
{
	context = new WebRequestContext();
	context->handle = curl_easy_init();
	if(!context->handle)
		throw WebException(T("Failed to initialize curl."));

	// Setup error message buffer
	memset(context->errorBuffer, 0, CURL_ERROR_SIZE);
	curl_easy_setopt(context->handle, CURLOPT_ERRORBUFFER, context->errorBuffer);
}

WebRequest::~WebRequest(void)
{
	WebRequestManager::getInstance()->closeRequest(context);

	if(context->handle)
	{
		// Close curl session
		curl_easy_cleanup(context->handle);
		context->handle = NULL;
	}
	delete context;
}

shared_ptr<WebRequest> WebRequest::create( const String & url )
{
	return shared_ptr<WebRequest>(new WebRequest(url));
}

bool WebRequest::wait(const int timeOutMilliseconds)
{
	return context->completed.wait(timeOutMilliseconds);
}

void WebRequest::abort()
{
	// Closing it will effectively cancel the current request
	WebRequestManager::getInstance()->closeRequest(context);
}

void WebRequest::downloadAsync(DataReceivedDelegate callback)
{
	jassert(callback != NULL);

	context->callback = callback;

	if( curl_easy_setopt(context->handle, CURLOPT_URL, (const char*)url) != CURLE_OK)
		handleError();

	// Follow redirects
	if( curl_easy_setopt(context->handle, CURLOPT_FOLLOWLOCATION, 1) != CURLE_OK)
		handleError();

	// Setup the read callback
	if( curl_easy_setopt(context->handle, CURLOPT_WRITEFUNCTION, receiveData) != CURLE_OK)
		handleError();

	// Set the custom data to be passed to the callback
	if( curl_easy_setopt(context->handle, CURLOPT_WRITEDATA, this) != CURLE_OK)
		handleError();

	WebRequestManager::getInstance()->beginRequest(context);
}

// Called when data is received
size_t WebRequest::receiveDataInternal(void* ptr, uint32 receivedBytes)
{
	// Is total length not known?
	if(totalBytes < 0)
	{
		// Determine content length
		double length = -1;
		if( curl_easy_getinfo(context->handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length) != CURLE_OK )
			handleError();
		if(length >= receivedBytes)
			totalBytes = (int64)length;
	}

	// Pass it on to the desired callback method
	shared_ptr<DataReceivedEventArgs> args( new DataReceivedEventArgs( ptr, receivedBytes, totalBytes ) );
	context->callback(args);

	// If callback wants to cancel transfer we return 0 to curl to make it stop
	if(args->cancelTransfer)
		return 0;

	// Return bytes handled to make curl continue
	return receivedBytes;
}

void WebRequest::handleError()
{
	// Wrap up the error message and throw an exception with the error
	String errmsg(context->errorBuffer);
	throw WebException(errmsg);
}

int WebRequest::getResponseCode()
{
	long responseCode = 0;
	if( curl_easy_getinfo(context->handle, CURLINFO_RESPONSE_CODE, &responseCode) != CURLE_OK )
		handleError();
	return responseCode;
}

