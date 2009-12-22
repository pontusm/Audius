#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebRequestManager.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles all ongoing connections for the application. Ensures that web communication
// is properly initialized.
//
////////////////////////////////////////////////////////////////////////////////////////

//#include <boost/shared_ptr.hpp>

#include "juce.h"

class WebRequest;
struct WebRequestContext;

class WebRequestManager : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(WebRequestManager, true)

	boost::shared_ptr<WebRequest> createRequest(const String & url);

	void beginRequest(WebRequestContext* request);
	void closeRequest(WebRequestContext* request);

private:
	WebRequestManager(void);
	~WebRequestManager(void);

	class impl;
	impl* vars;
};
