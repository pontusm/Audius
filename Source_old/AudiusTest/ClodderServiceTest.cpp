#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

void GetClodderCredentials(String& login, String& password)
{
	// The file "credentials.xml" must exist and should contain your credentials for running tests
	// requiring authentication. Make a copy of the file "credentials.tmpl.xml" and update it with your data.
	//DBG(File::getCurrentWorkingDirectory().getFullPathName());
	String fname(File::getCurrentWorkingDirectory().getFullPathName() + "/../../Source/AudiusTest/credentials.xml");
	File file(fname);
	WIN_ASSERT_TRUE(file.existsAsFile());

	XmlDocument xd(file);
	WIN_ASSERT_NOT_NULL(xd.getDocumentElement());
	XmlElement* xe = xd.getDocumentElement()->getChildByName("clodder");
	WIN_ASSERT_NOT_NULL(xe);

	login = xe->getStringAttribute("login");
	password = xe->getStringAttribute("password");
}

// ** Disabling these tests for now. Clodder service is not working...

//BEGIN_TEST(Services_ClodderService_CanGetCurrent)
//{
//	ClodderService svc;
//
//	String login;
//	String password;
//	GetClodderCredentials(login, password);
//
//	bool ok = svc.login(login, password);
//	WIN_ASSERT_TRUE(ok);
//
//	String current = svc.getCurrentPlaylistItem();
//	WIN_ASSERT_TRUE(current.length() > 0);
//	WIN_ASSERT_TRUE(current.indexOf(T("<s")) >= 0);
//}
//END_TEST

//BEGIN_TEST(Services_ClodderService_CanGetSongUrl)
//{
//	ClodderService svc;
//
//	String login;
//	String password;
//	GetClodderCredentials(login, password);
//
//	bool ok = svc.login(login, password);
//	WIN_ASSERT_TRUE(ok);
//
//	String url = svc.getSongUrl(1234);
//	WIN_ASSERT_TRUE(url.length() > 0);
//	WIN_ASSERT_TRUE(url.indexOf(T("1234")) >= 0);
//	WIN_ASSERT_TRUE(url.indexOf(T("st.ashx")) >= 0);
//}
//END_TEST

//BEGIN_TEST(Services_ClodderService_CanSearch)
//{
//	ClodderService svc;
//
//	String login;
//	String password;
//	GetClodderCredentials(login, password);
//
//	bool ok = svc.login(login, password);
//	WIN_ASSERT_TRUE(ok);
//
//	// ** Temporarily disabling this test **
//	//String result = svc.searchRaw(T("rage"));
//	//WIN_ASSERT_TRUE(result.length() > 0);
//	//WIN_ASSERT_TRUE(result.indexOf(T("Rage Against The Machine")) >= 0);
//}
//END_TEST
