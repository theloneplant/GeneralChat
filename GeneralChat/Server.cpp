#include "Global.h"
#include "Server.h"

int Server::init()
{
	#ifdef _WIN32
	// Init for Windows
	// Set up WSA data for Winsock
	WSADATA WsaDat;
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		error("Winsock error - Initialization failed \n");
		return -1;
	}

	// Create socket
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == INVALID_SOCKET)
	{
		error("Winsock error - Socket creation failed \n");
		return -1;
	}

	// Create server socket
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(Global::PORT);
	if (bind(serverSock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		error("Winsock error - Winsock bind failed \n");
		return -1;
	}

	// Max number of incoming requests
	listen(serverSock, 5);
	#else
	// Init for Unix
	// Create socket
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock < 0)
	{
		error("Socket creation failed \n");
		return -1;
	}

	// Set socket information
	bzero((char *)&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(Global::PORT);

	// Bind socket to the port
	if (bind(serverSock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		error("ERROR on binding");
	}

	// Max number of incoming requests
	listen(serverSock,5);
	#endif
	return 0;
}

// Read and Write have different function headers for each OS
#ifdef _WIN32
void Server::readClient(SOCKET client)
{
	// Start a running loop to constantly poll for a given client
	bool reading = true;
	while (reading)
	{
		// Check if still running
		if (!Global::running)
		{
			reading = false;
		}

		// Make a temp buffer
		char buf[Global::MAX_LENGTH];
		bzero(buf, Global::MAX_LENGTH);

		// Attempt to receive data from the client
		errorNum = recv(client, buf, Global::MAX_LENGTH - 1, 0);

		// If there was a conflict, resolve it
		if (errorNum < 0)
		{
			if (Global::running)
			{
				error("Error reading from socket");
			}
			else
			{
				// If the program is exiting, break out and clean up sockets
				error("Disconnected from server");
				reading = false;
				break;
			}
		}
		else if (errorNum == 0)
		{
			// Connection closed, break out of loop and close connection
			reading = false;
		}

		// Now that we have a message, attempt to send it to every other client
		Global::serverLock.lock();
		std::map<SOCKET, std::thread *>::const_iterator it;
		for (it = clients.begin(); it != clients.end(); it++)
		{
			if (it->first != client)
			{
				if (writeClient(it->first, buf))
				{
					// Write worked properly
				}
				else
				{
					// Connection closed, break out of loop and close connection
					reading = false;
				}
			}
		}
		Global::serverLock.unlock();
	}

	// Send a disconnect message
	FullMessage *message = new FullMessage("Server", "User disconnected", Color::WHITE);
	message->type = Notification::NEW_LOCAL_MESSAGE;
	Global::messageQueue.push(message);

	// Close connection
	std::lock_guard<std::mutex> lock(Global::serverLock);
	closesocket(client);
	clients.erase(client);
}

bool Server::writeClient(SOCKET client, const char *message)
{
	// Attempt to send a message to a given client
	errorNum = send(client, message, (int)strlen(message), 0);

	// If there was a conflict, resolve it
	if (errorNum < 0)
	{
		error("Error writing to socket");
		return false;
	}
	else if (errorNum == 0)
	{
		// Connection was closed
		return false;
	}
	return true;
}

#else
void Server::readClient(int client)
{
	// Start a running loop to constantly poll for a given client
	bool reading = true;
	while (reading)
	{
		// Check if still running
		if (!Global::running)
		{
			reading = false;
		}

		// Make a temp buffer
		char buf[Global::MAX_LENGTH];
		bzero(buf, Global::MAX_LENGTH);

		// Attempt to receive data from the client
		errorNum = read(client, buf, Global::MAX_LENGTH - 1);

		// If there was a conflict, resolve it
		if (errorNum < 0)
		{
			if (Global::running)
			{
				error("Error reading from socket");
			}
			else
			{
				error("Disconnected from server");
				reading = false;
				break;
			}
		}
		else if (errorNum == 0)
		{
			// Connection closed, break out of loop and close connection
			reading = false;
		}

		// Now that we have a message, attempt to send it to every other client
		Global::serverLock.lock();
		std::map<int, std::thread *>::iterator it;
		for (it = clients.begin(); it != clients.end(); it++)
		{
			if (it->first != client)
			{
				if (writeClient(it->first, buf))
				{
					// Write worked properly
				}
				else
				{
					// Connection closed, break out of loop and close connection
					reading = false;
				}
			}
		}
		Global::serverLock.unlock();
	}

	// Send a disconnect message
	std::string temp = "User disconnected";
	FullMessage *message = new FullMessage("Server", temp, Color::WHITE);
	message->type = Notification::NEW_LOCAL_MESSAGE;
	Global::messageQueue.push(message);

	// Close connection
	std::lock_guard<std::mutex> lock(Global::serverLock);
	#ifdef _WIN32
	closesocket(client);
	#else
	close(client);
	#endif
	clients.erase(client);
}

bool Server::writeClient(int client, const char *message)
{
	// Attempt to send a message to a given client
	errorNum = send(client, message, (int)strlen(message), 0);

	// If there was a conflict, resolve it
	if (errorNum < 0)
	{
		error("Error writing to socket");
		return false;
	}
	else if (errorNum == 0)
	{
		// Connection was closed
		return false;
	}
	return true;
}

#endif

void Server::acceptClients()
{
	// Poll for any new clients
	#ifdef _WIN32
	int sockSize = sizeof(client);
	SOCKET new_socket = accept(serverSock, (struct sockaddr *)&client, &sockSize);
	#else
	socklen_t sockSize = sizeof(client);
	int new_socket = accept(serverSock, (struct sockaddr *)&client, &sockSize);
	#endif

	// If there was an error, resolve it
	if (new_socket < 0)
	{
		error("Error, socket invalid \n");
	}
	else
	{
		// Otherwise, split the new client into its own thread
		std::thread *clientRead = new std::thread(&Server::readClient, this, new_socket);
		
		// And add it to our collection of clients & threads to keep track of later
		Global::serverLock.lock();
		#ifdef _WIN32
		clients.insert(std::pair<SOCKET, std::thread *>(new_socket, clientRead));
		#else
		clients.insert(std::pair<int, std::thread *>(new_socket, clientRead));
		#endif
		Global::serverLock.unlock();
	}
}

void Server::sendToAll(const char *message)
{
	// Go through each pair and write the message to every client
	Global::serverLock.lock();
	#ifdef _WIN32
	std::map<SOCKET, std::thread *>::iterator it;
	#else
	std::map<int, std::thread *>::iterator it;
	#endif
	for (it = clients.begin(); it != clients.end(); it++)
	{
		writeClient(it->first, message);
	}
	Global::serverLock.unlock();
}

void Server::closeServer()
{
	// Close each client
	#ifdef _WIN32
	std::map<SOCKET, std::thread *>::iterator it;
	#else
	std::map<int, std::thread *>::iterator it;
	#endif
	for (it = clients.begin(); it != clients.end(); it++)
	{
		#ifdef _WIN32
		closesocket(it->first);
		#else
		close(it->first);
		#endif
	}

	// Stop accepting new clients
	#ifdef _WIN32
	closesocket(serverSock);
	WSACleanup();
	#else
	close(serverSock);
	#endif
}

void Server::error(const char *message)
{
	closeServer();
	std::cout << message << std::endl;
}