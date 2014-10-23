#include "stdafx.h"
#include "FullMessage.h"
#include "Global.h"

FullMessage::FullMessage()
{
	username = message = "";
	color = WHITE;
}

FullMessage::FullMessage(std::string username, std::string message, Color color)
{
	// Initialize each variable
	FullMessage();
	this->message = message;
	this->username = username;
	this->color = color;

	// Estimate how high this message will be
	std::stringstream ss;
	ss << username << Global::DIVIDER << message;
	estimateWordWrap(ss.str());
}

std::string FullMessage::getMessage()
{
	return message;
}

std::string FullMessage::getUsername()
{
	return username;
}

Color FullMessage::getColor()
{
	return color;
}

int FullMessage::getHeight()
{
	return height;
}

void FullMessage::estimateWordWrap(std::string message)
{
	// Start index will be used as the index for the beginning of each line
	// End index is the end of the message
	int startIndex = 0;
	int endIndex = message.length() - 1;
	bool tooLong;
	height = 1; // Initialize height back to 1

	// While the remaining string is larger than the console width, word wrap
	while (endIndex - startIndex > Global::CONSOLE_WIDTH)
	{
		tooLong = true;

		// Update the end index of the current line
		int endOfNewLine = startIndex + Global::CONSOLE_WIDTH - 1;

		// Iterate backwards through the string, starting at the end of the current line
		for (int i = endOfNewLine; i >= startIndex; i--)
		{
			// Split the string after the first space
			if (message[i] == ' ')
			{
				startIndex = i + 1;
				tooLong = false;
				break;
			}
		}

		/*
			If the current word is longer than the line length, just wrap the 
			rest of the message

			This can be improved by only wrapping the long word, and continuing with
			the rest of the message
		*/
		if (tooLong)
		{
			int add = (endIndex - startIndex);
			height += add / Global::CONSOLE_WIDTH;
			break;
		}

		// Increment the height, since we just wrapped a new line
		height++;
	}
}