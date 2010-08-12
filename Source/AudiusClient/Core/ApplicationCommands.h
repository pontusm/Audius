#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: ApplicationCommands.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains definitions for all the available commands in the application.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

namespace ApplicationCommandIDs
{
	static const CommandID	play			= 0x2001;
	static const CommandID	pause			= 0x2002;
	static const CommandID	next			= 0x2003;
	static const CommandID	previous		= 0x2004;
	static const CommandID	toggleplaypause	= 0x2005;
	static const CommandID	refreshplaylist	= 0x2006;
}
