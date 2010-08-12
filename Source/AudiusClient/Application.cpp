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
			_logger = FileLogger::createDefaultAppLogger(appName, appName + T("Log.txt"), appName + T(" ") + getApplicationVersion());
			Logger::setCurrentLogger(_logger);

			Log::write(T("Initializing..."));

			// Init config
			ApplicationProperties::getInstance()->setStorageParameters(getApplicationName(), T("cfg"), String::empty, 3000, PropertiesFile::storeAsCompressedBinary);

			// Setup app context
			AppContext::getInstance()->initialise();

			// Load default skin
			//File skinpath = File::getCurrentWorkingDirectory().getChildFile("Skins/Default/");
			//SkinManager::getInstance()->initialise( skinpath );

			// Create music player
			AudioPlayer::getInstance()->initialise();

			// *** TEMP TEST - REMOVE!!! ******************************
			//if(!ServiceManager::getInstance()->getSpotify()->login(T("xxxx"), T("xxxx"), 5000))
			//	Log::write(T("Could not login to Spotify!"));

			//checkLogin();

			// Initialize main window
			_mainWindow = new MainWindow();
			//_mainWindow->centreWithSize(350, 170);
			//_mainWindow->setVisible(true);

			// Testing the animator
			//_animator = new ComponentAnimator();
			//_animator->animateComponent(_mainWindow, Rectangle(_mainWindow->getX(), _mainWindow->getY(), 500, 600), 1000, 3, 0);

			Log::write(T("Entering main loop"));
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
		Log::write(T("Shutting down..."));

		if(_mainWindow)
			_mainWindow->setVisible(false);

		DownloadManager::getInstance()->shutdown();
		AudioPlayer::getInstance()->shutdown();

		// *** TEMP TEST - REMOVE!!! ******************************
		//ServiceManager::getInstance()->getSpotify()->logout();

		ServiceManager::getInstance()->shutdown();

		// Release stuff
		//deleteAndZero(_animator);
		deleteAndZero(_mainWindow);

		AppContext::getInstance()->shutdown();

		// Write final log entry before exiting
		Log::write(T("Successfully terminated."));
		deleteAndZero(_logger);
	}

	const String getApplicationName()
	{
		return T("Audius");
	}

	const String getApplicationVersion()
	{
		return T("1.0 Alpha");
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
		Log::write(T("Fatal error!"));

		if(!e)
			return;

		Log::write(T("Unexpected exception: ") + String(e->what()) + T("(") + sourceFilename + T(", line ") + String(lineNumber) + T(")"));
	}


private:
	FileLogger*	_logger;
	MainWindow*	_mainWindow;

	//ComponentAnimator*	_animator;
};

START_JUCE_APPLICATION (AudiusApp)

