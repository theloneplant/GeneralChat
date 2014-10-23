/*
CharMessage represents an updated character from
the keyboard. It triggers an update of the user's
unfinished message.

This communicates closely with KeyboardThread and
ViewThread
*/

#ifndef CHAR_MESSAGE
#define CHAR_MESSAGE

#include "stdafx.h"
#include "Message.h"
#include "Color.h"

class CharMessage : public Message
{
private:
	char recent;

public:
	CharMessage();
	CharMessage(char recent);
	std::string getMessage();
	int getHeight();
};

#endif