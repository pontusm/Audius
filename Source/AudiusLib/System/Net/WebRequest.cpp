#include "Precompiled.h"
#include "WebRequest.h"

#include "WebRequestContext.h"
#include "WebRequestManager.h"
#include "WebException.h"
#include "HttpUtility.h"

using namespace boost;

WebRequest::WebRequest(const String & url) :
	url(url),
	totalBytes(-1),
	started(false),
	completed(false),
	postdataBuffer(NULL),
	cookieBuffer(NULL)
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
	//DBG(T("Closing request: ") + url);

	WebRequestManager* mgr = WebRequestManager::getInstanceWithoutCreating();
	if(mgr)
		mgr->closeRequest(this);

	if(context->handle)
	{
		// Close curl session
		curl_easy_cleanup(context->handle);
		context->handle = NULL;
	}
	delete context;

	deleteAndZero(postdataBuffer);
	deleteAndZero(cookieBuffer);
}

bool WebRequest::wait(const int timeOutMilliseconds)
{
	jassert(started);	// Should not wait for non started request

	if(completed)
		return true;
	return completeEvent.wait(timeOutMilliseconds);
}

void WebRequest::abort()
{
	// Closing it will effectively cancel the current request
	WebRequestManager::getInstance()->closeRequest(this);
}

void WebRequest::downloadAsync(DataReceivedDelegate callback)
{
	jassert(callback != NULL);
	jassert(!started);			// Cannot reuse request

	context->callback = callback;

	setupRequest();

	started = true;

	WebRequestManager::getInstance()->beginRequest(this);
}

void WebRequest::postAsync( const StringPairArray & parameters, DataReceivedDelegate callback )
{
	jassert(callback != NULL);
	jassert(!started);			// Cannot reuse request
	jassert(postdataBuffer == NULL);

	// Build parameter list
	String postdata;
	StringArray keys = parameters.getAllKeys();
	for(int i = 0; i < keys.size(); i++)
	{
		if(i > 0)
			postdata += '&';
		postdata += keys[i];
		postdata += '=';
		postdata += HttpUtility::urlEncode( parameters[keys[i]] );
	}

	// Determine utf8 length and then copy to buffer
	int bufflen = strlen( postdata.toUTF8() ) + 1;
	postdataBuffer = new uint8[bufflen];
	postdata.copyToUTF8(postdataBuffer, bufflen);

	context->callback = callback;

	setupRequest();

	// Use POST
	if( curl_easy_setopt(context->handle, CURLOPT_POST, 1) != CURLE_OK)
		handleError();

	// Setup data to post
	if( curl_easy_setopt(context->handle, CURLOPT_POSTFIELDS, postdataBuffer) != CURLE_OK)
		handleError();

	started = true;

	WebRequestManager::getInstance()->beginRequest(this);

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

void WebRequest::setComplete()
{
	completed = true;
	completeEvent.signal();
}


void WebRequest::setupRequest()
{
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

	// Add cookies
	if(cookies.size() > 0)
	{
		deleteAndZero(cookieBuffer);

		// Build cookie list
		String cookielist;
		StringArray keys = cookies.getAllKeys();
		for(int i = 0; i < keys.size(); i++)
		{
			if(i > 0)
				cookielist += "; ";
			cookielist += keys[i];
			cookielist += '=';
			cookielist += cookies[keys[i]];
		}

		// Determine utf8 length and then copy to buffer
		int bufflen = cookielist.length();
		cookieBuffer = new char[bufflen + 1];
		cookielist.copyToBuffer(cookieBuffer, bufflen);

		if( curl_easy_setopt(context->handle, CURLOPT_COOKIE, cookieBuffer) != CURLE_OK)
			handleError();
	}
}