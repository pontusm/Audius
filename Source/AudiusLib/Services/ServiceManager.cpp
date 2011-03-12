#include "Precompiled.h"
#include "ServiceManager.h"

#include "ClodderService.h"

ServiceManager::ServiceManager(void) :
	_clodderService(std::shared_ptr<ClodderService>( new ClodderService() )),
	_spotifyService(std::shared_ptr<SpotifyService>( new SpotifyService() ))
{
}

ServiceManager::~ServiceManager(void)
{
	clearSingletonInstance();
}

// Singleton impl
juce_ImplementSingleton(ServiceManager)
