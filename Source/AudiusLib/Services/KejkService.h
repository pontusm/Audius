#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: KejkService.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles communication with the Kejk web service.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class KejkService
{
public:
	KejkService(void);
	~KejkService(void);

	/** Login to the Kejk web service
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
