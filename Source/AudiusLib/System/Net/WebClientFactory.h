#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebClientFactory.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles the creation of web client instances and ensures that web communication is
// properly initialized.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/shared_ptr.hpp>

#include "juce.h"

class WebClient;

class WebClientFactory : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(WebClientFactory, true)

private:
	WebClientFactory(void);
	~WebClientFactory(void);

public:
	boost::shared_ptr<WebClient> createClient();
};
