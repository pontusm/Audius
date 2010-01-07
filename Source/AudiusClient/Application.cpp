////////////////////////////////////////////////////////////////////////////////////////
// File: Application.cpp
////////////////////////////////////////////////////////////////////////////////////////
//
// This is the main application class that handles startup and shutdown.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"

#include "ApplicationCommands.h"
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
		// Initialize logger
		String appName(getApplicationName());
		_logger = FileLogger::createDefaultAppLogger(appName, appName + T("Log.txt"), appName + T(" ") + getApplicationVersion());
		Logger::setCurrentLogger(_logger);

		try
		{
			Logger::writeToLog(T("Initializing..."));

			ApplicationProperties::getInstance()->setStorageParameters(getApplicationName(), T("cfg"), String::empty, 3000, PropertiesFile::storeAsCompressedBinary);

			// Create music player
			AudioPlayer::getInstance()->initialise();

			checkLogin();

			// Initialize main window
			_mainWindow = new MainWindow();
			//_mainWindow->centreWithSize(350, 170);
			//_mainWindow->setVisible(true);

			// Testing the animator
			//_animator = new ComponentAnimator();
			//_animator->animateComponent(_mainWindow, Rectangle(_mainWindow->getX(), _mainWindow->getY(), 500, 600), 1000, 3, 0);

			Logger::writeToLog(T("Entering main loop"));
		}
		catch(Exception& ex)
		{
			Logger::writeToLog(ex.getFullMessage());
			quit();
		}
	}

	void shutdown()
	{
		Logger::writeToLog(T("Shutting down..."));

		DownloadManager::getInstance()->shutdown();
		AudioPlayer::getInstance()->shutdown();

		// Release stuff
		//deleteAndZero(_animator);
		deleteAndZero(_mainWindow);

		// Write final log entry before exiting
		Logger::writeToLog(T("Successfully terminated."));
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

	virtual void unhandledException(const std::exception* e, const String& sourceFilename, const int lineNumber)
	{
		if(!e)
			return;

		Logger::writeToLog(T("Fatal error!"));
		Logger::writeToLog(T("Unexpected exception: ") + String(e->what()) + T("(") + sourceFilename + T(", line ") + String(lineNumber) + T(")"));
	}

	void getAllCommands(Array <CommandID>& commands)
	{
		commands.add(ApplicationCommandIDs::play);
		commands.add(ApplicationCommandIDs::pause);
		commands.add(ApplicationCommandIDs::next);
		commands.add(ApplicationCommandIDs::previous);
		commands.add(ApplicationCommandIDs::toggleplaypause);
		commands.add(ApplicationCommandIDs::refreshplaylist);

		// Register commands in base class
		JUCEApplication::getAllCommands(commands);
	}

	void getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
	{
		Player::Status status = AudioPlayer::getInstance()->getPlayerStatus();
		switch(commandID)
		{
		case ApplicationCommandIDs::play:
			result.setInfo(T("Play"), T("Start playing music"), T("Player"), status == Player::Playing ? ApplicationCommandInfo::isDisabled : 0);
			break;
		case ApplicationCommandIDs::pause:
			result.setInfo(T("Pause"), T("Pause music"), T("Player"), status != Player::Playing ? ApplicationCommandInfo::isDisabled : 0);
			result.addDefaultKeypress(KeyPress::stopKey, ModifierKeys());
			break;
		case ApplicationCommandIDs::next:
			result.setInfo(T("Next"), T("Go to next song"), T("Player"), 0);
			result.addDefaultKeypress(KeyPress::fastForwardKey, ModifierKeys());
			break;
		case ApplicationCommandIDs::previous:
			result.setInfo(T("Previous"), T("Go to previous song"), T("Player"), 0);
			result.addDefaultKeypress(KeyPress::rewindKey, ModifierKeys());
			break;
		case ApplicationCommandIDs::toggleplaypause:
			result.setInfo(T("Play/pause"), T("Play or pause music"), T("Player"), 0);
			result.addDefaultKeypress(KeyPress::playKey, ModifierKeys());
			break;
		case ApplicationCommandIDs::refreshplaylist:
			result.setInfo(T("Refresh playlist"), T("Refreshes the playlist from the server"), T("Player"), 0);
			break;
		default:
			JUCEApplication::getCommandInfo(commandID, result);
		}
	}

	bool perform(const InvocationInfo& info)
	{
		AudioPlayer* player = AudioPlayer::getInstance();
		switch(info.commandID)
		{
		case ApplicationCommandIDs::play:
			if(!checkLogin())
				return true;
			player->startPlaying();
			return true;
		case ApplicationCommandIDs::pause:
			player->pausePlaying();
			return true;
		case ApplicationCommandIDs::next:
			player->goToNext();
			return true;
		case ApplicationCommandIDs::previous:
			player->goToPrevious();
			return true;
		case ApplicationCommandIDs::toggleplaypause:
			if(player->getPlayerStatus() != Player::Playing && !checkLogin())
				return true;
			if(player->getPlaylist()->getCount() == 0)
				player->refreshPlaylist();

			player->togglePlayPause();
			return true;
		case ApplicationCommandIDs::refreshplaylist:
			player->refreshPlaylist();
			AlertWindow::showMessageBox(AlertWindow::InfoIcon, T("Refresh playlist"), T("Your playlist has been refreshed from the server."));
			return true;
		}
		return JUCEApplication::perform(info);
	}

	bool checkLogin()
	{
		if(ServiceManager::getInstance()->isLoggedIn())
			return true;

		// Show login dialog
		LoginComponent lc;
		int result = DialogWindow::showModalDialog(T("Login"), &lc, NULL, Colours::azure, true);
		if(result == 0)
			return false;	// User canceled

		if(!ServiceManager::getInstance()->getClodder()->login(lc.getLogin(), lc.getPassword()))
		{
			// Login failed
			Logger::writeToLog("Could not start playing. Login failed.");
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, T("Login failed."), T("Could not start playing. Login failed."));
			return false;
		}

		return true;
	}

private:
	FileLogger*	_logger;
	MainWindow*	_mainWindow;

	ComponentAnimator*	_animator;
};

START_JUCE_APPLICATION (AudiusApp)

