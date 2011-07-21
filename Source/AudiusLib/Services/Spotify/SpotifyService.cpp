#include "Precompiled.h"
#include "SpotifyService.h"

#include "SpotifyController.h"

#define DEFAULT_TIMEOUT_MSEC	10000

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

bool SpotifyService::login( const String & userName, const String & password, int timeOutMillisecs )
{
	_operationComplete.reset();
	SpotifyCallbackDelegate callback = boost::bind(&SpotifyService::signalOperationComplete, this);
	loginAsync(userName, password, callback);
	if(!_operationComplete.wait(timeOutMillisecs))
		return false;
	return _controller->isLoggedIn();
}

void SpotifyService::logout( int timeOutMillisecs /*= -1*/ )
{
	_operationComplete.reset();
	SpotifyCallbackDelegate callback = boost::bind(&SpotifyService::signalOperationComplete, this);
	logoutAsync(callback);
	_operationComplete.wait(timeOutMillisecs);
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

bool SpotifyService::isLoggedIn()
{
	return _controller->isLoggedIn();
}

void SpotifyService::searchAsync( const String & query, SpotifyEventDelegate callback )
{
	_controller->search(query, callback);
}

void SpotifyService::search( const String & query )
{
	_operationComplete.reset();
	SpotifyEventDelegate callback = boost::bind(&SpotifyService::signalOperationComplete, this);
	searchAsync(query, callback);
	_operationComplete.wait(DEFAULT_TIMEOUT_MSEC);
}
