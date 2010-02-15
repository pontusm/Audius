#include "stdafx.h"
#include "WinUnit.h"

#include "AudiusLib/AudiusLib.h"

void GetSpotifyCredentials(String& login, String& password)
{
	// The file "credentials.xml" must exist and should contain your credentials for running tests
	// requiring authentication. Make a copy of the file "credentials.tmpl.xml" and update it with your data.
	//DBG(File::getCurrentWorkingDirectory().getFullPathName());
	String fname(File::getCurrentWorkingDirectory().getFullPathName() + T("/../../Source/AudiusTest/credentials.xml"));
	File file(fname);
	WIN_ASSERT_TRUE(file.existsAsFile());

	XmlDocument xd(file);
	WIN_ASSERT_NOT_NULL(xd.getDocumentElement());
	XmlElement* xe = xd.getDocumentElement()->getChildByName(T("spotify"));
	WIN_ASSERT_NOT_NULL(xe);

	login = xe->getStringAttribute(T("login"));
	password = xe->getStringAttribute(T("password"));
}

WaitableEvent g_loginDone;
void loginHandler()
{
	g_loginDone.signal();
}

WaitableEvent g_logoutDone;
void logoutHandler()
{
	g_logoutDone.signal();
}

/* Commenting this out for now due to some Juce init problems...
BEGIN_TEST(Services_SpotifyService_CanLoginAsync)
{
	ScopedJuceInitialiser_NonGUI jc;

	String login;
	String password;
	GetSpotifyCredentials(login, password);

	SpotifyService svc;

	SpotifyCallbackDelegate callback = boost::bind(&loginHandler);
	svc.loginAsync(login, password, callback);
	WIN_ASSERT_TRUE( g_loginDone.wait( juce_isRunningUnderDebugger() ? INFINITE : 5000) );

	svc.logoutAsync(boost::bind(&logoutHandler));
	WIN_ASSERT_TRUE( g_logoutDone.wait(juce_isRunningUnderDebugger() ? INFINITE : 5000) );

	svc.shutdown();
}
END_TEST
*/
