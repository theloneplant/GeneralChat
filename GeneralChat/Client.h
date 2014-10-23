/*
Client is the object for a client connection to a server.
It will be constantly polling on its own thread for any data
from the server. If there is data to push out, it will be 
handled in ViewThread

This communicates closely with ClientThread and ViewThread
*/

#ifndef CLIENT
#define CLIENT

#include "stdafx.h"
#include "OS.h"

// Define some shortcut functions
#define bzero(b,len) (memset(( b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

class Client
{
private:
	#ifdef _WIN32
	SOCKET sock;
	#else
	int sock;
	#endif
	int errorNum;

public:
	int init(const char *hostName);
	std::string readServer();
	void writeServer(const char *message);
	void closeClient();
	void error(const char *message);
};

#endif