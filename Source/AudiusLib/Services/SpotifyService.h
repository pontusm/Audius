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

	void loginAsync(const String & userName, const String & password, SpotifyCallbackDelegate callback);
	void logoutAsync(SpotifyCallbackDelegate callback);

	void shutdown();

private:

private:
	SpotifyController* _controller;
};
