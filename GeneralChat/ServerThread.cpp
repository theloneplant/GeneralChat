#include "stdafx.h"
#include "Global.h"
#include "ServerThread.h"

int ServerThread::init()
{
	// Initialize the server
	return Global::server.init();
}

void ServerThread::start()
{
	// Start running loop
	Global::running = true;
	serverThread();
}

void ServerThread::stop()
{
	// Break out of the while loop
	Global::running = false;
	Global::server.closeServer();
}

void ServerThread::serverThread()
{
	while (Global::running)
	{
		// Continue to accept new clients
		Global::server.acceptClients();
	}
}