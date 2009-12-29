#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: WebRequestContext.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Used internally for storing information about a web request.
// This class is intended to hide implementation details to avoid exposing curl
// datatypes to clients that are including the normal WebRequest header.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "DataReceivedEventArgs.h"

#include <curl/curl.h>

struct WebRequestContext
{
	WebRequestContext() :
		handle(NULL)
	{
	}

	CURL*	handle;
	char	errorBuffer[CURL_ERROR_SIZE];

	DataReceivedDelegate	callback;

};
