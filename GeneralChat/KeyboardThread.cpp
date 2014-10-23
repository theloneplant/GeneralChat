#include "stdafx.h"
#include "Global.h"
#include "OS.h"
#include "KeyboardThread.h"
#include "KeyboardBuffer.h"
#include "CharMessage.h"
#include "FullMessage.h"

void KeyboardThread::start()
{
	// Begin running loop
	#ifdef _WIN32
	#else
	conioInit();
	#endif
	Global::running = true;
	keyboardThread();
}

void KeyboardThread::stop()
{
	// Break out of the while loop
	Global::running = false;
}

void KeyboardThread::keyboardThread()
{
	while (Global::running)
	{
		// Wait for input, then add input to the keyboard buffer
		// and notify observers that a character was received
		current = _getch();
		pushToQueue();
	}
}

void KeyboardThread::pushToQueue()
{
	Message *output;

	//std::cout << std::endl << current << (int) current << std::endl;

	if (current == Global::ESC) // If it's escape
	{
		stop();
	}
	else if (current == '`' || current == '\t')
	{
		// Skip, it's an illegal character
	}
	else if (current == Global::ENTER) // If it's a carriage return
	{
		if (Global::keyboardBuffer.getLength() > 0)
		{
			// Create a new message to send, this will be converted int a string later
			output = new FullMessage(Global::username, Global::keyboardBuffer.getMessage()->getMessage(), Global::color);
			output->type = Notification::NEW_LOCAL_MESSAGE;

			// Push it out to the MessageQueue
			Global::messageQueue.push(output);
			Global::keyboardBuffer.reset();
		}
	}
	else if (current == Global::BACKSPACE) // If it's a backspace
	{
		// Delete last char
		Global::keyboardBuffer.pop();

		output = new CharMessage(Global::keyboardBuffer.getRecentChar());
		output->type = Notification::NEW_CHAR;

		Global::messageQueue.push(output);
	}
	else
	{
		// Try to add character to the buffer, then send it if successful
		if (Global::keyboardBuffer.add(current))
		{
			output = new CharMessage(Global::keyboardBuffer.getRecentChar());
			output->type = Notification::NEW_CHAR;

			Global::messageQueue.push(output);
		}
	}
}