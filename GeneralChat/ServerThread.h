/*
ServerThread is used to let the server constantly
poll for new clients. It's optional when running the
program since it can act as either a client or 
server.

This communicates closely with Server
*/

#ifndef SERVER_THREAD
#define SERVER_THREAD

#include "Global.h"
#include "Server.h"

class ServerThread
{
private:
	void serverThread();

public:
	ServerThread()
	{
		Global::running = false;
	}
	int init();
	void start();
	void stop();
};

#endif