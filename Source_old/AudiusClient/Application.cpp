////////////////////////////////////////////////////////////////////////////////////////
// File: Application.cpp
////////////////////////////////////////////////////////////////////////////////////////
//
// This is the main application class that handles startup and shutdown.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"

#include "Core/AppContext.h"
#include "Core/ApplicationCommands.h"
#include "Gui/Main/MainWindow.h"
#include "Gui/Login/LoginComponent.h"

#include "../AudiusLib/AudiusLib.h"

class AudiusApp : public JUCEApplication
{
public:
	AudiusApp() :
	  _mainWindow(NULL),
	  _logger(NULL)
	{}

	~AudiusApp()
	{}

	void initialise( const String& commandLine )
	{
		try
		{
			// Initialize logger
			String appName(getApplicationName());
			_logger = FileLogger::createDefaultAppLogger(appName, appName + "Log.txt", appName + " " + getApplicationVersion());
			Logger::setCurrentLogger(_logger);

			Log::write("Initializing...");

			// Setup app context
			AppContext::getInstance()->initialise();

			// Load default skin
			//File skinpath = File::getCurrentWorkingDirectory().getChildFile("Skins/Default/");
			//SkinManager::getInstance()->initialise( skinpath );

			// *** TODO: This is temp test code that needs to be changed!
			// Login dialog should not be shown here. User needs to add music services and
			// we will try to login to all of them at this point without any prompt.
			checkLogin();

			// Initialize main window
			_mainWindow = new MainWindow();
			//_mainWindow->centreWithSize(350, 170);
			//_mainWindow->setVisible(true);

			// Testing the animator
			//_animator = new ComponentAnimator();
			//_animator->animateComponent(_mainWindow, Rectangle(_mainWindow->getX(), _mainWindow->getY(), 500, 600), 1000, 3, 0);

			Log::write("Entering main loop");
		}
		catch(Exception& ex)
		{
			if(_logger != NULL)
				Log::write(ex.getFullMessage());
			quit();
		}
	}

	void shutdown()
	{
		Log::write("Shutting down...");

		if(_mainWindow)
			_mainWindow->setVisible(false);

		if(ServiceManager::getInstance()->isLoggedIn())
			ServiceManager::getInstance()->getSpotify()->logout();

		// Release stuff
		//deleteAndZero(_animator);
		deleteAndZero(_mainWindow);

		AppContext::getInstance()->shutdown();

		// Write final log entry before exiting
		Log::write("Successfully terminated.");
		deleteAndZero(_logger);
	}

	const String getApplicationName()
	{
		return "Audius";
	}

	const String getApplicationVersion()
	{
		return "1.0 Alpha";
	}

	bool moreThanOneInstanceAllowed()
	{
		return false;
	}

	void anotherInstanceStarted()
	{
	}

	virtual void unhandledException(const std::exception* e, const String& sourceFilename, int lineNumber)
	{
		Log::write("Fatal error!");

		if(!e)
			return;

		Log::write("Unexpected exception: " + String(e->what()) + "(" + sourceFilename + ", line " + String(lineNumber) + ")");
	}

	bool checkLogin()
	{
		if(ServiceManager::getInstance()->isLoggedIn())
			return true;

		// Show login dialog
		LoginComponent lc;
		int result = DialogWindow::showModalDialog("Login", &lc, NULL, Colours::azure, true);
		if(result == 0)
			return false;	// User canceled

		if(!ServiceManager::getInstance()->getSpotify()->login(lc.getLogin(), lc.getPassword()))
		{
			// Login failed
			Logger::writeToLog("Could not start playing. Login failed.");
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Login failed.", "Could not start playing. Login failed.");
			return false;
		}

		return true;
	}

private:
	FileLogger*	_logger;
	MainWindow*	_mainWindow;

	//ComponentAnimator*	_animator;
};

START_JUCE_APPLICATION (AudiusApp)

