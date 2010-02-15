#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: ServiceManager.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Manages all supported remote services.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "SpotifyService.h"
#include "ClodderService.h"

class ServiceManager : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(ServiceManager, true)

	boost::shared_ptr<ClodderService> getClodder() { return _clodderService; }

	bool isLoggedIn()
	{
		return _clodderService->isLoggedIn();
	}

private:
	ServiceManager(void);
	~ServiceManager(void);

	boost::shared_ptr<ClodderService> _clodderService;
};
