#include "Global.h"
#include "CharMessage.h"

CharMessage::CharMessage()
{
	recent = ' ';
}

CharMessage::CharMessage(char recent)
{
	this->recent = recent;
}

std::string CharMessage::getMessage()
{
	// getMessage must always be a string
	std::stringstream ss;
	ss << recent;
	return ss.str();
}

int CharMessage::getHeight()
{
	// Ask the keyboardBuffer for its height
	return Global::keyboardBuffer.getHeight();
}