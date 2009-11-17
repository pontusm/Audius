#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: MainWindow.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles the main window of the application.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class AppTrayIconComponent;
class MusicPlayer;

class MainWindow : public DocumentWindow
{
public:
	MainWindow(MusicPlayer* player);
	~MainWindow(void);

	void closeButtonPressed()
	{
		JUCEApplication::quit();
	}

private:
	bool hookMediaKeys();
	void unhookMediaKeys();

private:
	AppTrayIconComponent*	_trayIcon;
	ApplicationCommandManager* _appCommandManager;
};
