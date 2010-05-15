#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: Exception.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// A generic exception class.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class Exception : public std::exception
{
public:
	Exception(const String& message, Exception* innerException = NULL) :
		_message(message),
		_innerException(innerException)
	{
	}

	virtual ~Exception()
	{
		if(_innerException)
			delete _innerException;
	}

	const String& getMessage() const { return _message; }
	Exception* getInnerException() const { return _innerException; }

	/** Retrieves the full error message, including all the inner exceptions. */
	String getFullMessage()
	{
		String msg(_message);

		Exception* ex = _innerException;
		while(ex != NULL)
		{
			msg += T(" ") + ex->getMessage();
			ex = ex->getInnerException();
		}
		return msg;
	}

	virtual const char* what() const throw()
	{
		return getMessage().toCString();
	}

protected:
	String		_message;
	Exception*	_innerException;
};
