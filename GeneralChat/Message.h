/*
Message is an abstract class that generalizes
any message that is sent through the MessageQueue.

This is never initialized, it's abstract
*/

#ifndef MESSAGE
#define MESSAGE

#include "Notification.h"

class Message
{
public:
	Notification type;

	virtual std::string getMessage() = 0;
	virtual int getHeight() = 0;
};

#endif