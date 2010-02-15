#include "Precompiled.h"
#include "SpotifyService.h"

#include "SpotifyController.h"


SpotifyService::SpotifyService(void) :
	_controller(new SpotifyController() )
{
	if(_controller->initialize())
		_controller->startThread(0);
}

SpotifyService::~SpotifyService(void)
{
	shutdown();
}

void SpotifyService::loginAsync( const String & userName, const String & password, SpotifyCallbackDelegate callback )
{
	_controller->login(userName, password, callback);
}

void SpotifyService::logoutAsync( SpotifyCallbackDelegate callback )
{
	_controller->logout(callback);
}

void SpotifyService::shutdown()
{
	if(_controller)
	{
		_controller->shutdown();

		deleteAndZero(_controller);
	}

}
