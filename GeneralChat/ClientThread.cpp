#include "stdafx.h"
#include "Global.h"
#include "Formatter.h"
#include "ClientThread.h"
#include "Message.h"
#include "FullMessage.h"

int ClientThread::init(std::string hostName)
{
	// Initialize the global client
	return Global::client.init(hostName.c_str());
}

void ClientThread::start()
{
	// Start running loop
	Global::running = true;
	clientThread();
}

void ClientThread::stop()
{
	// Break out of the while loop & clean up
	Global::running = false;
	Global::client.closeClient();
}

void ClientThread::clientThread()
{
	while (Global::running)
	{
		// Wait for a message
		std::string str = Global::client.readServer();

		// Double check the string size, dont let garbage in
		if (str.size() > 0)
		{
			// Convert the received string into a FullMessage, then output it
			FullMessage *output = Formatter::formatRead(str);
			output->type = Notification::NEW_MESSAGE;

			Global::messageQueue.push(output);
		}
	}
}