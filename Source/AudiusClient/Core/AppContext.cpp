#include "Precompiled.h"
#include "AppContext.h"

#include "CommandManager.h"

#include "../../AudiusLib/AudiusLib.h"

AppContext::AppContext(void)
{
}

AppContext::~AppContext(void)
{
}

void AppContext::initialise()
{
	// Init preferences
	PropertiesFile::Options options;
	options.applicationName = "Audius";
	options.filenameSuffix = ".cfg";
	properties.setStorageParameters(options);

	// Setup global managers
	commandManager = new CommandManager();

	// Init services
	AudioPlayer::getInstance()->initialise();
}

void AppContext::shutdown()
{
	// Shutdown services
	DownloadManager::getInstance()->shutdown();
	AudioPlayer::getInstance()->shutdown();

	ServiceManager::getInstance()->shutdown();

	properties.closeFiles();

	// Release stuff
	deleteAndZero(commandManager);
}

// Singleton impl
juce_ImplementSingleton(AppContext)
