#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: SpotifyService.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles communication with the Spotify music service.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include "SpotifyEventArgs.h"

class SpotifyController;

class SpotifyService
{
public:
	SpotifyService(void);
	~SpotifyService(void);

	bool isLoggedIn();

	bool login(const String & userName, const String & password, int timeOutMillisecs = -1);
	void logout(int timeOutMillisecs = -1);

	void loginAsync(const String & userName, const String & password, SpotifyCallbackDelegate callback);
	void logoutAsync(SpotifyCallbackDelegate callback);

	void shutdown();

private:
	void signalOperationComplete() { _operationComplete.signal(); }

private:
	SpotifyController* _controller;

	WaitableEvent _operationComplete;
};
