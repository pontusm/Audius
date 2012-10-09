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

class MainWindow : public DocumentWindow,
				   public ActionListener
{
public:
	MainWindow();
	~MainWindow(void);

	void closeButtonPressed()
	{
		JUCEApplication::quit();
	}

private:
	bool hookMediaKeys();
	void unhookMediaKeys();

	virtual void actionListenerCallback(const String& message);

private:
	AppTrayIconComponent*	_trayIcon;
	ApplicationCommandManager* _appCommandManager;
};
