#include "Precompiled.h"
#include "MainWindow.h"

#include "../../ApplicationCommands.h"
#include "../AppTrayIconComponent.h"
#include "MainComponent.h"
//#include "PlayerComponent.h"

#include "../../../AudiusLib/AudiusLib.h"

using namespace boost;

// Globally shared data segment
#pragma data_seg(".HOOKDATA")
HHOOK g_hook = NULL;
#pragma data_seg()

#pragma comment(linker, "/SECTION:.HOOKDATA,RWS")

ApplicationCommandManager* g_commandManager = NULL;

MainWindow::MainWindow() :
	DocumentWindow(T("Audius"),
					Colour(0xffbee6c8),
					DocumentWindow::minimiseButton | DocumentWindow::closeButton,
					true),
	_appCommandManager(NULL),
	_trayIcon(NULL)
{
	// Init commands
	_appCommandManager = new ApplicationCommandManager();
	_appCommandManager->registerAllCommandsForTarget(JUCEApplication::getInstance());
	g_commandManager = _appCommandManager;

	// Setup key mappings
	//_appCommandManager->getKeyMappings()->resetToDefaultMappings();
	//_appCommandManager->getKeyMappings()->restoreFromXml (lastSavedKeyMappingsXML);		// Get user defined key mappings
	// Pass on any keys received by this window to the command manager
	addKeyListener(_appCommandManager->getKeyMappings());

	// Hook media keys
	hookMediaKeys();

	// Setup main window component (lifetime is managed by window)
	//PlayerComponent* component = new PlayerComponent(player, _appCommandManager);
	MainComponent* component = new MainComponent(_appCommandManager);
	setContentComponent(component, true, true);

	//setResizeLimits(200, 170, 1000, 170);
	setResizable(true, false);

	//centreWithSize(350, 170);
	centreWithSize(500, 500);

	//centreWithSize(getWidth(), getHeight());

	setVisible(true);

#if JUCE_WIN32 || JUCE_LINUX
	_trayIcon = new AppTrayIconComponent(_appCommandManager);
#endif

	AudioPlayer::getInstance()->addActionListener(this);
}

MainWindow::~MainWindow(void)
{
	AudioPlayer::getInstance()->removeActionListener(this);

#if JUCE_WIN32 || JUCE_LINUX
	deleteAndZero (_trayIcon);
#endif

	unhookMediaKeys();

	// This will ensure that the current content component and all its children
	// are deleted before we destroy the command manager which is used by it
	setContentComponent(NULL, true);

	g_commandManager = NULL;
	deleteAndZero(_appCommandManager);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION && wParam == WM_KEYDOWN && g_commandManager)
	{
		KBDLLHOOKSTRUCT* kbdinfo = (KBDLLHOOKSTRUCT*)lParam;
		CommandID cmd = 0;
		switch(kbdinfo->vkCode)
		{
		case VK_MEDIA_PLAY_PAUSE:
			cmd = ApplicationCommandIDs::toggleplaypause;
			break;
		case VK_MEDIA_NEXT_TRACK:
			cmd = ApplicationCommandIDs::next;
			break;
		case VK_MEDIA_PREV_TRACK:
			cmd = ApplicationCommandIDs::previous;
			break;
		case VK_MEDIA_STOP:
			cmd = ApplicationCommandIDs::pause;
			break;
		}

		if(cmd != 0)
		{
			g_commandManager->invokeDirectly(cmd, true);
			return TRUE;
		}
	}
	return ( CallNextHookEx(g_hook, nCode, wParam, lParam) );
}

bool MainWindow::hookMediaKeys()
{
	g_hook = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), NULL);
	if(!g_hook)
	{
		Log::write(T("Failed to hook media keys."));
		return false;
	}

	return true;
}

void MainWindow::unhookMediaKeys()
{
	if(g_hook)
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = NULL;
	}
}

void MainWindow::actionListenerCallback( const String& message )
{
	if(message == PlayerNotifications::newSong)
	{
		shared_ptr<SongInfo> songInfo = AudioPlayer::getInstance()->getCurrentSong();
		setName(songInfo->getTitle() + T(" - ") + songInfo->getArtist());
	}
}
