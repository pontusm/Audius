#include "Precompiled.h"
#include "ServiceManager.h"

#include "ClodderService.h"

using namespace boost;

ServiceManager::ServiceManager(void) :
	_clodderService(shared_ptr<ClodderService>( new ClodderService() )),
	_spotifyService(shared_ptr<SpotifyService>( new SpotifyService() ))
{
}

ServiceManager::~ServiceManager(void)
{
	clearSingletonInstance();
}

// Singleton impl
juce_ImplementSingleton(ServiceManager)
