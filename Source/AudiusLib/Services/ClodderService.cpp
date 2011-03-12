#include "Precompiled.h"
#include "ClodderService.h"

#include "../System/Net/WebException.h"
#include "../System/Net/WebClient.h"
#include "../System/Net/HttpUtility.h"

ClodderService::ClodderService(void) :
	_baseUrl(T("http://clodder.com/"))
{
}

ClodderService::~ClodderService(void)
{
}

bool ClodderService::login( const String & userName, const String & password )
{
	_userKey = String::empty;

	String encodedUserName = HttpUtility::urlEncode(userName);
	String encodedPassword = HttpUtility::urlEncode(password);

	String url = _baseUrl + T("s/lgin.aspx?u=") + encodedUserName + T("&p=") + encodedPassword;

	WebClient client;
	String key = client.downloadString(url);
	if(key.length() != 36)
		return false;

	_userKey = key;
	return true;
}

String ClodderService::getCurrentPlaylistItem()
{
	String url = _baseUrl + T("s/c.aspx?uk=") + _userKey;
	WebClient client;
	return client.downloadString(url);
}

String ClodderService::getSongUrl(int songID)
{
	String url = _baseUrl + T("s/st.ashx?uk=") + _userKey + T("&i=") + String(songID);
	return url;
}

String ClodderService::gotoNext()
{
	String url = _baseUrl + T("s/n.aspx?uk=") + _userKey;
	WebClient client;
	return client.downloadString(url);
}

String ClodderService::gotoPrevious()
{
	String url = _baseUrl + T("s/p.aspx?uk=") + _userKey;
	WebClient client;
	return client.downloadString(url);
}

String ClodderService::searchRaw( const String & text )
{
	String url = _baseUrl + T("c/s2.aspx");
	StringPairArray params;
	params.set("q", text);
	WebClient client;
	StringPairArray cookies;
	cookies.set("uk", _userKey);
	client.setCookies(cookies);
	return client.post(url, params);
}
