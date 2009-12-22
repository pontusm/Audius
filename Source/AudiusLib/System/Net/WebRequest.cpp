#include "Precompiled.h"
#include "WebRequest.h"

#include "WebRequestContext.h"
#include "WebRequestManager.h"
#include "WebException.h"

using namespace boost;

WebRequest::WebRequest(const String & url) :
	url(url)
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

void WebRequest::downloadAsync(DataReceivedDelegate callback)
{
	jassert(callback != NULL);

	context->callback = callback;

	if( curl_easy_setopt(context->handle, CURLOPT_URL, (const char*)url) != 0)
		handleError();

	// Setup the read callback
	if( curl_easy_setopt(context->handle, CURLOPT_WRITEFUNCTION, receiveData) != 0)
		handleError();

	// Set the custom data to be passed to the callback
	if( curl_easy_setopt(context->handle, CURLOPT_WRITEDATA, this) != 0)
		handleError();

}

// Called when data is received
size_t WebRequest::receiveDataInternal(void* ptr, uint32 receivedBytes)
{
	// Return bytes handled to make curl continue
	return receivedBytes;
}

void WebRequest::handleError()
{
	// Wrap up the error message and throw an exception with the error
	String errmsg(context->errorBuffer);
	throw WebException(errmsg);
}
