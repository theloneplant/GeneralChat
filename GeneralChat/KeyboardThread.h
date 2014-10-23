/*
KeyboardThread is in charge of all user input. When
the user presses a key, it's added onto a buffer.

When the user hits ENTER, a message is sent out and
the buffer is flushed.

This communicates closely with KeyboardBuffer
*/

#ifndef KEYBOARD_THREAD
#define KEYBOARD_THREAD

#include "Global.h"
#include "KeyboardBuffer.h"

class KeyboardThread
{
private:
	char current;

	void keyboardThread();
	virtual void pushToQueue();

public:
	KeyboardThread()
	{
		Global::running = false;
		current = ' ';
	}
	void start();
	void stop();
};

#endif