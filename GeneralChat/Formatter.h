#ifndef FORMATTER
#define FORMATTER

#include "stdafx.h"
#include "Global.h"
#include "FullMessage.h"

namespace Formatter
{
	static const char *formatWrite(FullMessage *message)
	{
		char *formatted = new char[Global::MAX_LENGTH];
		
		// Set the color at the first character
		std::map<char, Color>::const_iterator it;
		for (it = Global::colorMap.begin(); it != Global::colorMap.end(); it++)
		{
			if (it->second == message->getColor())
			{
				formatted[0] = it->first;
			}
		}

		// Set the username directly after
		for (unsigned int i = 0; i < message->getUsername().length(); i++)
		{
			formatted[i + 1] = message->getUsername()[i];
		}

		// Insert dividing character between username and message
		int messageStart = 2 + message->getUsername().length();
		formatted[messageStart - 1] = '`';

		// Read until end of message
		for (unsigned int i = 0; i < message->getMessage().length(); i++)
		{
			formatted[messageStart + i] = message->getMessage()[i];
		}

		// Set the last 2 characters of the message, truncate if necessary
		int endOfMessage = messageStart + message->getMessage().length();
		if (endOfMessage <= Global::MAX_LENGTH - 2)
		{
			formatted[endOfMessage] = '\n';
			formatted[endOfMessage] = '\0';
		}
		else
		{
			formatted[Global::MAX_LENGTH - 2] = '\n';
			formatted[Global::MAX_LENGTH - 1] = '\0';
		}
		return formatted;
	}

	static FullMessage *formatRead(std::string str)
	{
		Color color = Color::WHITE;
		std::string username, message;

		// Set the color at the first character
		std::map<char, Color>::const_iterator it;
		for (it = Global::colorMap.begin(); it != Global::colorMap.end(); it++)
		{
			if (it->first == str[0])
			{
				color = it->second;
			}
		}

		// Look for the dividing character '`'
		int divIndex = 0;
		for (unsigned int i = 0; i < str.length(); i++)
		{
			if (str[i] == '`')
			{
				divIndex = i;
				break;
			}
		}

		// Set the username and message
		username = str.substr(1, divIndex - 1);
		message = str.substr(divIndex + 1, str.length());

		// Create a FullMessage with our username, message, and color
		FullMessage *formatted = new FullMessage(username, message, color);
		return formatted;
	}
}

#endif