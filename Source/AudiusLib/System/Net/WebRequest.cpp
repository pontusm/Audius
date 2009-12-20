#include "Precompiled.h"
#include "WebRequest.h"

#include "WebRequestContext.h"
#include "WebRequestManager.h"
#include "WebException.h"

WebRequest::WebRequest()
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

boost::shared_ptr<WebRequest> WebRequest::create( const String & url )
{
	return WebRequestManager::getInstance()->createRequest(url);
}
