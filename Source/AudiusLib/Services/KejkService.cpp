#include "Precompiled.h"
#include "KejkService.h"

#include "../System/Net/WebException.h"
#include "../System/Net/WebClientFactory.h"
#include "../System/Net/WebClient.h"

using namespace boost;

KejkService::KejkService(void) :
	_baseUrl(T("http://kejk.se/s"))
{
}

KejkService::~KejkService(void)
{
}

String KejkService::login( const String & userName, const String & password )
{
	String encodedUserName = WebClient::urlEncode(userName);
	String encodedPassword = WebClient::urlEncode(password);

	String url = _baseUrl + T("/lgin.aspx?u=") + encodedUserName + T("&p=") + encodedPassword;

	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	String key = client->downloadString(url);
	if(key.length() == 0)
	{
		String msg(T("User name or password is incorrect."));
		throw WebException(msg);
	}
	return key;
}

String KejkService::getCurrentPlaylistItem( const String & userKey )
{
	String url = _baseUrl + T("/c.aspx?uk=") + userKey;
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	return client->downloadString(url);
}

String KejkService::getSongUrl( const String & userKey, int songID )
{
	String url = _baseUrl + T("/st.ashx?uk=") + userKey + T("&i=") + String(songID);
	return url;
}

String KejkService::gotoNext( const String & userKey )
{
	String url = _baseUrl + T("/n.aspx?uk=") + userKey;
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	return client->downloadString(url);
}

String KejkService::gotoPrevious( const String & userKey )
{
	String url = _baseUrl + T("/p.aspx?uk=") + userKey;
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	return client->downloadString(url);
}
