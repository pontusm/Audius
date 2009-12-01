#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

void GetClodderCredentials(String& login, String& password)
{
	// The file "credentials.xml" must exist and should contain your credentials for running tests
	// requiring authentication. Make a copy of the file "credentials.tmpl.xml" and update it with your data.
	//DBG(File::getCurrentWorkingDirectory().getFullPathName());
	String fname(File::getCurrentWorkingDirectory().getFullPathName() + T("/../../Source/AudiusTest/credentials.xml"));
	File file(fname);
	WIN_ASSERT_TRUE(file.existsAsFile());

	XmlDocument xd(file);
	WIN_ASSERT_NOT_NULL(xd.getDocumentElement());
	XmlElement* xe = xd.getDocumentElement()->getChildByName(T("clodder"));
	WIN_ASSERT_NOT_NULL(xe);

	login = xe->getStringAttribute(T("login"));
	password = xe->getStringAttribute(T("password"));
}

BEGIN_TEST(Services_ClodderService_CanGetCurrent)
{
	ClodderService svc;

	String login;
	String password;
	GetClodderCredentials(login, password);

	String key = svc.login(login, password);
	WIN_ASSERT_TRUE(key.length() == 36);
	WIN_ASSERT_TRUE(key.indexOf(T("-")) >= 0);

	String current = svc.getCurrentPlaylistItem(key);
	WIN_ASSERT_TRUE(current.length() > 0);
	WIN_ASSERT_TRUE(current.indexOf(T("<s")) >= 0);

	//<?xml version="1.0" encoding="utf-8"?><s n="City on a Hill" a="Genghis Tron" l="5" i="3671" s="3972677" c="0" t="206" />
}
END_TEST

BEGIN_TEST(Services_ClodderService_CanGetSongUrl)
{
	ClodderService svc;

	String login;
	String password;
	GetClodderCredentials(login, password);

	String key = svc.login(login, password);
	WIN_ASSERT_TRUE(key.length() == 36);
	WIN_ASSERT_TRUE(key.indexOf(T("-")) >= 0);

	String url = svc.getSongUrl(key, 1234);
	WIN_ASSERT_TRUE(url.length() > 0);
	WIN_ASSERT_TRUE(url.indexOf(T("1234")) >= 0);
	WIN_ASSERT_TRUE(url.indexOf(T("st.ashx")) >= 0);
}
END_TEST
