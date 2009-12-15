#include "Precompiled.h"
#include "ClodderService.h"

#include "../System/Net/WebException.h"
#include "../System/Net/WebClientFactory.h"
#include "../System/Net/WebClient.h"

using namespace boost;

ClodderService::ClodderService(void) :
	_baseUrl(T("http://clodder.com/s"))
{
}

ClodderService::~ClodderService(void)
{
}

bool ClodderService::login( const String & userName, const String & password )
{
	_userKey = String::empty;

	String encodedUserName = WebClient::urlEncode(userName);
	String encodedPassword = WebClient::urlEncode(password);

	String url = _baseUrl + T("/lgin.aspx?u=") + encodedUserName + T("&p=") + encodedPassword;

	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	String key = client->downloadString(url);
	if(key.length() != 36)
		return false;

	_userKey = key;
	return true;
}

String ClodderService::getCurrentPlaylistItem()
{
	String url = _baseUrl + T("/c.aspx?uk=") + _userKey;
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	return client->downloadString(url);
}

String ClodderService::getSongUrl(int songID)
{
	String url = _baseUrl + T("/st.ashx?uk=") + _userKey + T("&i=") + String(songID);
	return url;
}

String ClodderService::gotoNext()
{
	String url = _baseUrl + T("/n.aspx?uk=") + _userKey;
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	return client->downloadString(url);
}

String ClodderService::gotoPrevious()
{
	String url = _baseUrl + T("/p.aspx?uk=") + _userKey;
	shared_ptr<WebClient> client = WebClientFactory::getInstance()->createClient();
	return client->downloadString(url);
}
