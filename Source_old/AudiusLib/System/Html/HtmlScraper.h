#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: HtmlScraper.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// A utility class for scraping HTML contents. This scraper is primitive and does not
// handle all possible cases and badly formatted HTML. Use carefully... :)
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class HtmlScraper
{
public:
	HtmlScraper(const String & html) :
		_html(html),
		_position(0)
	{
	}
	~HtmlScraper(void)
	{
	}

	// Returns the current element name. Must be at an element.
	String getElementName();

	// Retrieves everything within the start and end tag. Must be at an element.
	String getElementContents();

	// Retrieve the contents of an attribute. Must be at an element.
	String getAttribute(const String & attributeName);

	// Tries to locate the next element in the document
	bool gotoNextElement();

	void gotoStart() { _position = 0; }
	void gotoEnd() { _position = _html.length(); }

	bool isAtStart() { return _position == 0; }
	bool isAtEnd() { return _position == _html.length(); }
	bool isAtElement() { return _html[_position] == '<'; }

	// Tries to find the next element with the specified name
	bool findNextElement(const String & elementName);

	// Tries to find a specific element with a certain attribute content
	bool findNextElement(const String & elementName, const String & content);

private:
	//int findElementEndPosition();
	//int findNextElementPosition();

private:
	int _position;

	String	_html;
};
