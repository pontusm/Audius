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

#include "Spotify/SpotifyService.h"
#include "ClodderService.h"

class ServiceManager : public DeletedAtShutdown
{
public:
	juce_DeclareSingleton(ServiceManager, true)

	boost::shared_ptr<ClodderService> getClodder() { return _clodderService; }
	boost::shared_ptr<SpotifyService> getSpotify() { return _spotifyService; }

	bool isLoggedIn()
	{
		return _clodderService->isLoggedIn();
	}

	void shutdown()
	{
		_spotifyService->shutdown();
	}

private:
	ServiceManager(void);
	~ServiceManager(void);

	boost::shared_ptr<ClodderService> _clodderService;
	boost::shared_ptr<SpotifyService> _spotifyService;
};
