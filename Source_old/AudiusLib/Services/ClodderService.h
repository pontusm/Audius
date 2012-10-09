#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: ClodderService.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles communication with the Clodder web service.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class ClodderService
{
public:
	ClodderService(void);
	~ClodderService(void);

	/** Login to the Clodder web service
	@param userName		The account user name
	@param password		The password for the account.

	@returns			True if login successful.
	*/
	bool login(const String & userName, const String & password);

	String getCurrentPlaylistItem();
	String getSongUrl(int songID);

	String gotoNext();
	String gotoPrevious();

	String searchRaw(const String & text);

	String getUserKey() { return _userKey; }
	bool isLoggedIn() { return _userKey.length() > 0; }

private:
	String	_baseUrl;
	String	_userKey;
};
