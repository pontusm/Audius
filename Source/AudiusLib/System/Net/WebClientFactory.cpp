#include "Precompiled.h"
#include "WebClientFactory.h"

#include "WebClient.h"

#include <curl/curl.h>

using namespace boost;

WebClientFactory::WebClientFactory(void)
{
	DBG(T("WebClientFactory init"));

	// Init curl
	curl_global_init(CURL_GLOBAL_ALL);
}

WebClientFactory::~WebClientFactory(void)
{
	DBG(T("WebClientFactory shutdown"));

	// Shutdown curl
	curl_global_cleanup();

	clearSingletonInstance();
}

shared_ptr<WebClient> WebClientFactory::createClient()
{
	return shared_ptr<WebClient>( new WebClient() );
}

// Singleton impl
juce_ImplementSingleton(WebClientFactory)
