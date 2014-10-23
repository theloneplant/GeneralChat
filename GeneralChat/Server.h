/*
Server is the class that handles all communications between
clients. It is in charge of who is connected and what is
being sent. This is optional to use at startup.

Each instance of this program has the opportunity to be a
server. If it chooses to be one, this class will be initialized
and started.

This communicates closely with ServerThread
*/

#ifndef SERVER
#define SERVER

#include "stdafx.h"
#include "OS.h"

// Define some shortcut functions
#ifdef _WIN32
#define bzero(b,len) (memset(( b), '\0', (len)), (void) 0)
#endif
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

class Server
{
private:
	#ifdef _WIN32
	std::map<SOCKET, std::thread *> clients;
	SOCKET serverSock;
	struct sockaddr_in server, client;
	#else
	std::map<int, std::thread *> clients;
	int serverSock;
	struct sockaddr_in server, client;
	#endif
	int errorNum;

public:
	int init();
	#ifdef _WIN32
	void readClient(SOCKET client);
	bool writeClient(SOCKET client, const char *message);
	#else
	void readClient(int client);
	bool writeClient(int client, const char *message);
	#endif
	void sendToAll(const char *message);
	void acceptClients();
	void closeServer();
	void error(const char *message);
};

#endif