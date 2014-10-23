#include "Global.h"
#include "Client.h"

int Client::init(const char *hostName)
{
	#ifdef _WIN32
	// Init for Windows
	// Setup WSA data for Winsock
	WSADATA WsaDat;
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		error("Winsock initialization failed \n");
		return -1;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		error("Socket creation failed \n");
		return -1;
	}

	// Resolve IP address for hostname
	struct hostent *host = gethostbyname(hostName);
	if (host == NULL)
	{
		error("Failed to resolve hostname \n");
		return -1;
	}

	// Set socket information
	SOCKADDR_IN sockAddr;
	sockAddr.sin_port = htons(Global::PORT);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = *((unsigned long *) host->h_addr);

	// Attempt to connect to server
	if (connect(sock, (SOCKADDR *)&sockAddr, sizeof(sockAddr)) != 0)
	{
		error("Failed to connect to server \n");
		return -1;
	}
	#else
	// Init for Unix
	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		error("Socket creation failed \n");
		return -1;
	}

	// Resolve IP address for hostname
	hostent *host = gethostbyname(hostName);
	if (host == NULL)
	{
		error("Failed to resolve hostname \n");
		return -1;
	}

	// Set socket information
	struct sockaddr_in sockAddr;
	bzero((char *)&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	bcopy((char *)host->h_addr, (char *)&sockAddr.sin_addr.s_addr, host->h_length);
	sockAddr.sin_port = htons(Global::PORT);

	// Attempt to connect to server
	if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0)
	{
		error("Failed to connect to server \n");
		return -1;
	}
	#endif
	return 0;
}

std::string Client::readServer()
{
	// Initialize a cstring
	char buf[Global::MAX_LENGTH];
	bzero(buf, Global::MAX_LENGTH);

	// Attempt to read from the socket
	#ifdef _WIN32
	errorNum = recv(sock, buf, Global::MAX_LENGTH - 1, 0);
	#else
	errorNum = read(sock, buf, Global::MAX_LENGTH - 1);
	#endif

	// If there was a conflict, resolve it
	if (errorNum < 0)
	{
		if (Global::running)
		{
			Global::running = false;
			return "WServer`Disconnected from server";
		}
		else
		{
			return "WServer`Disconnected from server";
		}
	}
	else if (errorNum == 0)
	{
		// Disconnected
		Global::running = false;
		return "WServer`Disconnected from server";
	}

	// Finish by converting the cstring to a string, just to make life a bit easier
	std::string message = buf;
	return message;
}

void Client::writeServer(const char *buffer)
{
	// Attempt a write to the socket
	#ifdef _WIN32
	errorNum = send(sock, buffer, (int) strlen(buffer), 0);
	#else
	errorNum = write(sock, buffer, strlen(buffer));
	#endif

	// If there was a conflict, resolve it
	if (errorNum < 0)
	{
		error("Error writing to socket");
		Global::running = false;
	}
	else if (errorNum == 0)
	{
		// Disconnected
		Global::running = false;
	}
}

void Client::closeClient()
{
	// Clean up any loose ends
	#ifdef _WIN32
	shutdown(sock, SD_SEND);
	WSACleanup();
	#else
	close(sock);
	#endif
}

void Client::error(const char *message)
{
	// Clean up and print an error
	closeClient();
	std::cout << message << std::endl;
}