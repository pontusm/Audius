#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: MessageQueue.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Implements a thread safe message queue.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

#include <queue>

class MessageQueue
{
public:
	virtual ~MessageQueue()
	{
		const ScopedLock l(_cs);
		// Clean up messages
		Message* message;
		while( (message = retrieveNextMessage()) != NULL)
		{
			delete message;
		}
	}

	/** Posts a message to the queue. The message will be deleted by the queue after it has been handled. */
	void postThreadMessage(Message* const message)
	{
		const ScopedLock l(_cs);
		_messages.push(message);
	}

	/** Processes all pending messages. All messages are passed
		to the handleMessage() method.
	*/
	void processPendingMessages()
	{
		Message* message;
		while( (message = retrieveNextMessage()) != NULL)
		{
			handleMessage(*message);
			delete message;
		}
	}

protected:
	/** Override this to take care of messages that are posted to the queue. */
	virtual void handleMessage(const Message& message) = 0;

private:
	Message* retrieveNextMessage()
	{
		// Enter critical section
		const ScopedLock l(_cs);
		if(_messages.size() == 0)
			return NULL;

		Message* message = _messages.front();
		_messages.pop();
		return message;
	}

private:
	CriticalSection _cs;

	std::queue<Message*>	_messages;
};