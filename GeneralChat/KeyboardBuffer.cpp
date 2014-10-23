#include "KeyboardBuffer.h"
#include "Global.h"

void KeyboardBuffer::reset()
{
	// Reset all variables
	buffer.clear();
	buffer.str("");
	recent = '\0';
}

bool KeyboardBuffer::add(char recent)
{
	// Check if this new char will overflow the buffer, then add it
	if (getLength() + 1 < Global::MAX_LENGTH)
	{
		buffer << recent;
		this->recent = recent;
		return true;
	}
	return false;
}

bool KeyboardBuffer::add(std::string &recent)
{
	// Check if this new string will overflow the buffer, then add it
	if (recent.length() > 0 && getLength() + recent.length() < Global::MAX_LENGTH)
	{
		buffer << recent;
		this->recent = recent[recent.length() - 1];
		return true;
	}
	return false;
}

void KeyboardBuffer::pop()
{
	// Verify that the buffer can be popped
	if (buffer.str().length() > 1)
	{
		// Pop the last char off the buffer, then reinitialize the buffer
		std::string temp = buffer.str();
		temp.erase(temp.length() - 1);
		reset();
		add(temp);
	}
	else
	{
		// Otherwise reset everything for good measure
		reset();
	}
}

FullMessage *KeyboardBuffer::getMessage()
{
	// Return the user's temporary message as a FullMessage
	// Say: message
	std::string user = "Say";
	FullMessage *message = new FullMessage(user, buffer.str(), Color::WHITE);
	return message;
}

int KeyboardBuffer::getHeight()
{
	// Initialize a new FullMessage to estimate its height
	std::string user = "Say";
	FullMessage message(user, buffer.str(), Color::WHITE);
	return message.getHeight();
}

char KeyboardBuffer::getRecentChar()
{
	return recent;
}

int KeyboardBuffer::getLength()
{
	// Return the buffer length
	return buffer.str().length();
}