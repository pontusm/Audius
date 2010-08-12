#include "Precompiled.h"
#include "AppContext.h"

#include "CommandManager.h"

AppContext::AppContext(void)
{
}

AppContext::~AppContext(void)
{
}

void AppContext::initialise()
{
	commandManager = new CommandManager();
}

void AppContext::shutdown()
{
	deleteAndZero(commandManager);
}

// Singleton impl
juce_ImplementSingleton(AppContext)
