/*
ClientThread is used for communication with the server.
This thread only handles network INPUT, not output.
Network output is taken care of by ViewThread, which
does output to both the console and network.

This communicates closely with Client
*/

#ifndef CLIENT_THREAD
#define CLIENT_THREAD

#include "Global.h"
#include "Client.h"

class ClientThread
{
private:
	void clientThread();

public:
	ClientThread()
	{
		Global::running = false;
	}
	int init(std::string hostName);
	void start();
	void stop();
};

#endif