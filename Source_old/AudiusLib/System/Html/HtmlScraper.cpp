#include "Precompiled.h"
#include "HtmlScraper.h"


bool HtmlScraper::gotoNextElement()
{
	if(isAtEnd())
		return false;
	int pos = _html.indexOfChar(_position + 1, '<');
	if(pos < 0)
		return false;
	_position = pos;
	return true;
}

String HtmlScraper::getElementName()
{
	jassert(isAtElement());

	int endpos = _html.indexOfAnyOf(" >", _position + 1);
	if(endpos < 0)
		return String::empty;

	String name = _html.substring(_position + 1, endpos);
	return name.trimCharactersAtEnd(" />");
}

String HtmlScraper::getAttribute( const String & attributeName )
{
	jassert(isAtElement());

	int endpos = _html.indexOf(_position + 1, ">");
	if(endpos < 0)
		return String::empty;

	int pos = _html.indexOf(_position + 1, attributeName + "=");
	if(pos < 0 || pos > endpos)
		return String::empty;

	int attributeStartPos = pos + attributeName.length() + 1;
	int attributeEndPos = _html.indexOfAnyOf(" >\"", attributeStartPos + 1);
	if(attributeEndPos < 0)
		return String::empty;

	String value = _html.substring(attributeStartPos, attributeEndPos).trimCharactersAtEnd(" >");
	return value.unquoted();
}

String HtmlScraper::getElementContents()
{
	jassert(isAtElement());

	// Locate start of content
	int startpos = _html.indexOfChar(_position + 1, '>');
	if(startpos < 0)
		return String::empty;

	startpos++;		// Skip left bracket

	// Locate end tag position
	String name = getElementName();
	int endpos = _html.indexOfIgnoreCase(startpos, "</" + name + ">");
	if(endpos < 0)
		return String::empty;

	return _html.substring(startpos, endpos);
}

bool HtmlScraper::findNextElement( const String & elementName )
{
	if(isAtEnd())
		return false;

	int pos = _html.indexOf(_position + 1, "<" + elementName);
	if(pos < 0)
		return false;
	_position = pos;
	return true;
}

bool HtmlScraper::findNextElement( const String & elementName, const String & content )
{
	if(isAtEnd())
		return false;

	int currpos = _position;

	while(currpos >= 0)
	{
		// Find element
		int pos = _html.indexOf(currpos + 1, "<" + elementName);
		if(pos < 0)
			return false;

		currpos = pos;

		int endpos = _html.indexOf(pos, ">");
		if(endpos < 0)
			return false;

		// Check if content matches
		String element = _html.substring(pos + elementName.length() + 1, endpos);
		if(element.indexOf(content) >= 0)
			break;
	}

	_position = currpos;
	return true;
}

