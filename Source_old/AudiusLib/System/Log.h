#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: Log.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// A convenience wrapper around the Juce logger providing some additional features.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class Log
{
private:
	Log() {}
	~Log() {}

public:
	// Writes to the default log
	static void write(const String & message)
	{
		String threadName;
		Thread* thread = Thread::getCurrentThread();
		if(thread)
			threadName = thread->getThreadName();

		Time now(Time::getCurrentTime());
		Logger::writeToLog(now.toString(false, true, true, true) +
							String(",") + String(now.getMilliseconds()) +
							String(" [") + threadName + String("] ") +
							message);
	}
};
