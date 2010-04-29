#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: SpotifyController.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Internal class that manages the current Spotify session.
// This class is intended to hide implementation details and avoid exposing
// internal data types to the clients of the SpotifyService.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined
#endif
#include "libspotify/include/libspotify/api.h"

#include "SpotifyKey.h"
#include "SpotifyService.h"
#include "SpotifyEventArgs.h"

#include "../../System/Exception.h"

class SpotifyController : public Thread
{
public:
	SpotifyController() :
		Thread(T("SpotifyController")),
		_nextTimeout(5000),
		_session(NULL)
	{
	}
	~SpotifyController()
	{
		// Release is not currently working...
		//if(_session)
		//	sp_session_release(_session);
	}

	// Thread entry point
	void run()
	{
		DBG("Spotify controller started");
		try
		{
			while(!threadShouldExit())
			{
				processPendingEvents();

				_pendingEvent.wait(_nextTimeout);
			}
		}
		catch (Exception& ex)
		{
			if(!threadShouldExit())
				Log::write(T("Spotify controller thread exception: ") + ex.getFullMessage());
		}
		DBG("Spotify controller shutdown");
	}

	bool isLoggedIn()
	{
		if(!_session)
			return false;
		return sp_session_user(_session) != NULL;
	}

	// Initialize the controller
	bool initialize()
	{
		// List of callback methods
		static sp_session_callbacks callbacks = {
			logged_in,
			logged_out,
			metadata_updated,
			connection_error,
			message_to_user,
			notify_main_thread,
			NULL,
			play_token_lost,
			log_message
		};

		sp_session_config config;

		// Setup config
		config.api_version = SPOTIFY_API_VERSION;
		config.cache_location = "_Temp";
		config.settings_location = "_Temp";
		config.application_key = g_spotifykey;
		config.application_key_size = g_spotifykey_size;
		config.user_agent = "Audius";
		config.callbacks = &callbacks;
		config.userdata = this;

		sp_error error = sp_session_init(&config, &_session);
		if(error != SP_ERROR_OK)
		{
			Log::write(T("Failed to initialize Spotify session. ") + String(sp_error_message(error)) );
			return false;
		}

		return true;
	}

	// Login to Spotify
	bool login(const String & userName, const String & password, SpotifyCallbackDelegate callback)
	{
		assert(callback != NULL);
		_loginCallback = callback;

		const ScopedLock l(_apiLock);
		sp_error error = sp_session_login(_session, userName, password);
		if(error != SP_ERROR_OK)
		{
			Log::write(T("Spotify login failed. ") + String(sp_error_message(error)) );
			return false;
		}
		return true;
	}

	// Logout from Spotify
	bool logout(SpotifyCallbackDelegate callback)
	{
		assert(callback != NULL);
		_logoutCallback = callback;

		const ScopedLock l(_apiLock);
		sp_error error = sp_session_logout(_session);
		if(error != SP_ERROR_OK)
		{
			Log::write(T("Spotify logout failed. ") + String(sp_error_message(error)) );
			return false;
		}
		return true;
	}

	void search(const String & query, SpotifyEventDelegate callback)
	{
		// TODO: Create search operation object (supply it with session+callback)
		//sp_search* search = sp_search_create(_session,
		//										query,
		//										0, 10,
		//										0, 5,
		//										0, 3,
		//										&search_complete, NULL);
	}

	void shutdown()
	{
		// Ensure graceful exit
		signalThreadShouldExit();
		_pendingEvent.signal();
		waitForThreadToExit(_nextTimeout);
	}

private:
	void processPendingEvents()
	{
		const ScopedLock l(_apiLock);
		sp_session_process_events(_session, &_nextTimeout);
	}


private:
	// Callback functions used by libspotify

	static void SP_CALLCONV connection_error(sp_session *session, sp_error error)
	{
		DBG("connection_error");
		Log::write(T("Spotify connection failed. ") + String(sp_error_message(error)) );
	}

	static void SP_CALLCONV logged_in(sp_session *session, sp_error error) {
		if (SP_ERROR_OK != error) {
			Log::write(T("Spotify login failed. ") + String(sp_error_message(error)) );
			return;
		}

		SpotifyController* self = static_cast<SpotifyController*>(sp_session_userdata(session));
		sp_user* user = sp_session_user(session);
		const char* my_name = (sp_user_is_loaded(user) ? sp_user_display_name(user) : sp_user_canonical_name(user));

		Log::write(T("Logged in to Spotify as user ") + String(my_name) );

		if(self->_loginCallback)
			self->_loginCallback();
	}

	static void SP_CALLCONV logged_out(sp_session *session)
	{
		DBG("logged_out");
		SpotifyController* self = static_cast<SpotifyController*>(sp_session_userdata(session));
		if(self->_logoutCallback)
			self->_logoutCallback();
	}


	static void SP_CALLCONV notify_main_thread(sp_session *session)
	{
		// Notify main thread to do more processing
		SpotifyController* self = static_cast<SpotifyController*>(sp_session_userdata(session));
		self->_pendingEvent.signal();
	}


	static void SP_CALLCONV log_message(sp_session *session, const char *data)
	{
		DBG("log_message");
		Log::write(T("Spotify log: ") + String(data) );
	}


	static void SP_CALLCONV metadata_updated(sp_session *session)
	{
		DBG("metadata_updated");
	}


	static void SP_CALLCONV message_to_user(sp_session *session, const char *data)
	{
		DBG("message_to_user");
		Log::write(T("Spotify user message: ") + String(data) );
	}


	static void SP_CALLCONV play_token_lost(sp_session *session)
	{
		DBG("play_token_lost");
		Log::write(T("Spotify play token lost."));
	}

private:
	WaitableEvent	_pendingEvent;
	CriticalSection	_apiLock;

	sp_session*	_session;

	int	_nextTimeout;

	SpotifyEventDelegate	_eventCallback;
	SpotifyCallbackDelegate _loginCallback;
	SpotifyCallbackDelegate _logoutCallback;
};

