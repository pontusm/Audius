#include "Precompiled.h"
#include "MainWindow.h"

#include "../AppTrayIconComponent.h"
#include "PlayerComponent.h"

#include "AudiusExt/AudiusExt.h"

MainWindow::MainWindow(MusicPlayer* player) :
	DocumentWindow(T("Audius"),
					Colours::azure,
					DocumentWindow::minimiseButton | DocumentWindow::closeButton,
					true),
	_appCommandManager(NULL),
	_trayIcon(NULL)
{
	// Init commands
	_appCommandManager = new ApplicationCommandManager();
	_appCommandManager->registerAllCommandsForTarget(JUCEApplication::getInstance());

	// Setup key mappings
	//_appCommandManager->getKeyMappings()->resetToDefaultMappings();
	//_appCommandManager->getKeyMappings()->restoreFromXml (lastSavedKeyMappingsXML);		// Get user defined key mappings
	// Pass on any keys received by this window to the command manager
	addKeyListener(_appCommandManager->getKeyMappings());

	// Hook media keys
	HookMediaKeys((HWND)getPeer()->getNativeHandle());

	// Setup main window component (lifetime is managed by window)
	PlayerComponent* pc = new PlayerComponent(player, _appCommandManager);
	setContentComponent(pc, true, true);

	setResizeLimits(200, 170, 1000, 170);
	setResizable(true, false);

	centreWithSize(350, 170);

	//centreWithSize(getWidth(), getHeight());

	setVisible(true);

#if JUCE_WIN32 || JUCE_LINUX
	_trayIcon = new AppTrayIconComponent(_appCommandManager);
#endif

}

MainWindow::~MainWindow(void)
{
#if JUCE_WIN32 || JUCE_LINUX
	deleteAndZero (_trayIcon);
#endif

	UnhookMediaKeys();

	// This will ensure that the current content component and all its children
	// are deleted before we destroy the command manager which is used by it
	setContentComponent(NULL, true);

	deleteAndZero(_appCommandManager);
}
