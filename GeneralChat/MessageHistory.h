/*
MessageHistory is a class that saves all submitted messages
locally for the program's view to use. This is used for setting
the cursor's position when drawing new messages.

This communicates closely with ViewThread
*/

#ifndef MESSAGE_HISTORY
#define MESSAGE_HISTORY

#include "stdafx.h"
#include "FullMessage.h"

class MessageHistory
{
private:
	int height;
	std::vector<FullMessage *> messages;

public:
	MessageHistory()
	{
		height = 0;
	}
	void add(FullMessage *message);
	int getHeight();
	void setHeight(int height);
};

#endif