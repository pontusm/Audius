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

	@returns			The user key for the session.
	*/
	String login(const String & userName, const String & password);

	String getCurrentPlaylistItem(const String & userKey);
	String getSongUrl(const String & userKey, int songID);

	String gotoNext(const String & userKey);
	String gotoPrevious(const String & userKey);

/*

	CString GetCurrentPlaylistItem(const CString& userkey);
	CString GoToNext(const CString& userkey);
	CString GoToPrevious(const CString& userkey);
	CString GetSongUrl(const CString& userkey, int songID);

private:
	CString m_baseUrl;
*/
private:
	String	_baseUrl;
};
