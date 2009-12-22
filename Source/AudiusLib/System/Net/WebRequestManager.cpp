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

// Singleton impl
juce_ImplementSingleton(WebRequestManager)
