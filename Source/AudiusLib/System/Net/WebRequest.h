#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebRequest.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Provides functionality for managing a single web request. Created through the
// WebRequestManager class.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

struct WebRequestContext;

class WebRequest
{
	friend class WebRequestManager;
private:
	WebRequest();

public:
	~WebRequest(void);

	static boost::shared_ptr<WebRequest> create(const String & url);

private:
	WebRequestContext*	context;
};
