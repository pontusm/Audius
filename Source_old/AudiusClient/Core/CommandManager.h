#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: CommandManager.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Handles the global commands for the application.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class CommandManager : public ApplicationCommandTarget
{
public:
	CommandManager(void);
	~CommandManager(void);

	void getAllCommands(Array <CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	bool perform(const InvocationInfo& info);

	ApplicationCommandTarget* getNextCommandTarget();

private:
	bool checkLogin();
};
