#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: AppContext.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Global singleton class that manages the current application state.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class CommandManager;

class AppContext : public DeletedAtShutdown
{
private:
	AppContext(void);
	~AppContext(void);

public:
	juce_DeclareSingleton(AppContext, true)

	void initialise();
	void shutdown();

	// Public fields
public:
	CommandManager* commandManager;
};
