#include "ViewThread.h"
#include "Formatter.h"

void ViewThread::start()
{
	Global::running = true;
	viewThread();
}

void ViewThread::stop()
{
	Global::running = false;
}

void ViewThread::viewThread()
{
	while (Global::running)
	{
		Message *message = Global::messageQueue.pop();

		// Clear Old Messages
		Global::cursor.setPosition(0, Global::messageHistory.getHeight());

		if (message->type == Notification::NEW_CHAR)
		{
			// Echo the updated message
			printWordWrap(Global::keyboardBuffer.getMessage());
		}
		else
		{
			FullMessage *fullMsg = dynamic_cast<FullMessage *>(message);

			// Output to screen if it's either a local or network message
			printWordWrap(fullMsg);

			// Add it to history
			Global::messageHistory.add(fullMsg);

			std::cout << std::endl;
			printWordWrap(Global::keyboardBuffer.getMessage());

			if (message->type == Notification::NEW_LOCAL_MESSAGE)
			{
				// If it's a locally typed message, send it to the server
				// Make sure to convert the message before sending
				Global::client.writeServer(Formatter::formatWrite(fullMsg));
			}
		}
		delete message;
	}
}

void ViewThread::clearLines(int height)
{
	// If we've hit the bottom of the input buffer, clear the screen
	// This is necessary because setting the cursor position behaves weird
	// when beyond the buffer size.
	if (Global::messageHistory.getHeight() + height > Global::BUFFER_HEIGHT)
	{
		Global::messageHistory.setHeight(0);
		system(Global::CLEAR.c_str());
	}

	// Travel down y lines, and clear them with spaces
	Global::cursor.setPosition(0, Global::messageHistory.getHeight());
	for (int y = 0; y < height; y++)
	{
		std::string clearChar = " ";
		for (int i = 0; i < Global::CONSOLE_WIDTH; i++)
		{
			print(clearChar);
		}
	}

	// Reset cursor position back up
	Global::cursor.setPosition(0, Global::messageHistory.getHeight());
}

void ViewThread::print(std::string message)
{
	// Just print the message...
	std::cout << message;
}

void ViewThread::printUsername(FullMessage *message)
{
	// Prints the username in its respective color, then resets back to white
	Global::cursor.setColor(message->getColor());
	print(message->getUsername());
	Global::cursor.setColor(Color::WHITE);
}

void ViewThread::printMessage(FullMessage *message)
{
	// Print the remainder after the username
	print(Global::DIVIDER + message->getMessage());
}

void ViewThread::printWordWrap(FullMessage *message)
{
	// Start index will be used as the index for the beginning of each line
	// End index is the end of the message
	std::string fullMsg = message->getUsername() + Global::DIVIDER + message->getMessage();
	int startIndex = 0;
	int endIndex = fullMsg.length() - 1;
	bool tooLong, usernameOutput = false;

	// Clear the message's height plus 1, because there may be artifacts from
	// pressing backspace
	clearLines(message->getHeight() + 1);

	// Print username
	printUsername(message);

	if (endIndex - startIndex < Global::CONSOLE_WIDTH)
	{
		// Print the message normally if it's too short to word wrap
		printMessage(message);
	}
	else
	{
		// While the remaining string is larger than the console width, word wrap
		while (endIndex - startIndex >= Global::CONSOLE_WIDTH)
		{
			// Reset tooLong
			tooLong = true;

			// Update the end index of the current line
			int endOfNewLine = startIndex + Global::CONSOLE_WIDTH - 1;

			if (!usernameOutput)
			{
				usernameOutput = true;
				startIndex = message->getUsername().length();
			}

			// Iterate backwards through the string, starting at the end of the current line
			for (int i = endOfNewLine; i >= startIndex; i--)
			{
				// Split the string after the first space
				if (fullMsg[i] == ' ')
				{
					std::string output = fullMsg.substr(startIndex, i - startIndex);
					print(output + "\n");

					startIndex = i + 1;
					tooLong = false;
					break;
				}
			}

			/*
			If the current word is longer than the line length, just wrap the
			rest of the message

			This can be improved by only wrapping the long word, and continuing with
			the rest of the message
			*/
			if (tooLong)
			{
				break;
			}
		}

		// Print last line of message
		std::string output = fullMsg.substr(startIndex, endIndex - startIndex + 1);
		print(output);
	}
}