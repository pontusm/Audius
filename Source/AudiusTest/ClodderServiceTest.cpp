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

	bool ok = svc.login(login, password);
	WIN_ASSERT_TRUE(ok);

	String current = svc.getCurrentPlaylistItem();
	WIN_ASSERT_TRUE(current.length() > 0);
	WIN_ASSERT_TRUE(current.indexOf(T("<s")) >= 0);
}
END_TEST

BEGIN_TEST(Services_ClodderService_CanGetSongUrl)
{
	ClodderService svc;

	String login;
	String password;
	GetClodderCredentials(login, password);

	bool ok = svc.login(login, password);
	WIN_ASSERT_TRUE(ok);

	String url = svc.getSongUrl(1234);
	WIN_ASSERT_TRUE(url.length() > 0);
	WIN_ASSERT_TRUE(url.indexOf(T("1234")) >= 0);
	WIN_ASSERT_TRUE(url.indexOf(T("st.ashx")) >= 0);
}
END_TEST
