#include "Precompiled.h"
#include "WebRequestManager.h"

#include "WebException.h"

#include "WebRequest.h"
#include "WebRequestContext.h"
#include "WebRequestController.h"

#include <curl/curl.h>

using namespace boost;


// ********************
// *** Private data ***
// ********************
class WebRequestManager::impl
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

	WebRequestController controller;

	//CURLM*	multiHandle;
};

// ******************************
// *** Constructor/destructor ***
// ******************************
WebRequestManager::WebRequestManager(void) :
	vars( new impl() )
{
	// Init curl
	curl_global_init(CURL_GLOBAL_ALL);

	CURLM* multiHandle = curl_multi_init();
	if(!multiHandle)
		throw WebException(T("Failed to initialize libcurl."));

	vars->controller.multiHandle = multiHandle;
}

WebRequestManager::~WebRequestManager(void)
{
	// Shutdown transfer thread
	vars->controller.stopThread(5000);

	// Shutdown curl
	if(vars->controller.multiHandle)
		curl_multi_cleanup(vars->controller.multiHandle);
	curl_global_cleanup();

	delete vars;
	clearSingletonInstance();
}

// TODO: Consolidate curl error handling
void handleError(CURLMcode err)
{
	String errmsg(curl_multi_strerror(err));
	throw WebException(errmsg);
}

void WebRequestManager::beginRequest(WebRequestContext* request)
{
	CURLMcode result = curl_multi_add_handle(vars->controller.multiHandle, request->handle);
	if(result != CURLM_OK)
		handleError(result);

	// Notify controller in case its asleep
	vars->controller.signalPendingRequest();
}

void WebRequestManager::closeRequest(WebRequestContext* request)
{
	CURLMcode result = curl_multi_remove_handle(vars->controller.multiHandle, request->handle);
	if(result != CURLM_OK)
		handleError(result);
}


// Singleton impl
juce_ImplementSingleton(WebRequestManager)
