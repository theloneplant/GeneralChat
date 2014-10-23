/*
Cursor is used to position the console cursor and change
its color when necessary. When a new message comes in, 
the cursor jumps to the start of the user's message, then
prints out any new messages from the server, then prints
out the user's current message again.

This communicates closely with ViewThread
*/

#ifndef CURSOR
#define CURSOR

#include "stdafx.h"
#include "Color.h"
#include "FullMessage.h"

class Cursor
{
public:
	void setColor(Color color);
	void setPosition(int x, int y);
};

#endif