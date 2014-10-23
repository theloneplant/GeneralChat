/*
ViewThread handles ALL forms of output. This
includes both console output AND network output.

Console output is printed by performing a word
wrapping algorithm to the message, and printing
out the colored username followed by a message.

Network output is done by determining if a message
is whole and if it came from this client. If it
did, convert it to a string and send it out.

This communicates closely with Cursor and 
MessageHistory
*/

#ifndef VIEW
#define VIEW

#include "Global.h"
#include "Cursor.h"
#include "KeyboardBuffer.h"
#include "MessageHistory.h"
#include "FullMessage.h"
#include "CharMessage.h"
#include "Notification.h"

class ViewThread
{
private:
	void viewThread();
	void print(std::string message);
	void printUsername(FullMessage *message);
	void printMessage(FullMessage *message);
	void printWordWrap(FullMessage *message);
	void clearLines(int height);
	
public:
	ViewThread()
	{
		Global::running = false;
	}
	void start();
	void stop();
};

#endif