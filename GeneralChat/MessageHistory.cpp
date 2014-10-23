#include "MessageHistory.h"

void MessageHistory::add(FullMessage *message)
{
	// Push another message onto the history, then update its height
	messages.push_back(message);
	height += message->getHeight();
}

int MessageHistory::getHeight()
{
	return height;
}

void MessageHistory::setHeight(int height)
{
	this->height = height;
}