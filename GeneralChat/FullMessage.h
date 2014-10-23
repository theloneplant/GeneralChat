/*
FullMessage is the main message type used to
communicate in the chat system. Whenever a
FullMessage is initialized, it estimates how tall
it will be when printed on the screen.

This communicates closely with KeyboardThread, 
ClientThread, and ViewThread
*/

#ifndef FULL_MESSAGE
#define FULL_MESSAGE

#include "stdafx.h"
#include "Message.h"
#include "Color.h"

class FullMessage : public Message
{
private:
	std::string message, username;
	Color color;
	int height;

	void estimateWordWrap(std::string message);

public:
	FullMessage();
	FullMessage(std::string username, std::string message, Color color);
	std::string getMessage();
	std::string getUsername();
	Color getColor();
	int getHeight();
};

#endif