/*
KeyboardBuffer is used to store the user's temporary messages.
This is the message that they type out, then send to the server
when finished by pressing ENTER.

This communicates closely with KeyboardThread
*/

#ifndef KEYBOARD_BUFFER
#define KEYBOARD_BUFFER

#include "stdafx.h"
#include "FullMessage.h"

class KeyboardBuffer
{
private:
	std::stringstream buffer;
	char recent;

public:
	void reset();
	bool add(char recent);
	bool add(std::string &recent);
	void pop();
	FullMessage *getMessage();
	int getHeight();
	char getRecentChar();
	int getLength();
};

#endif