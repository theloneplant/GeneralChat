#include "stdafx.h"
#include "OS.h"
#include "Global.h"
#include "Server.h"
#include "KeyboardThread.h"
#include "ClientThread.h"
#include "ServerThread.h"
#include "ViewThread.h"

bool isServer();
void initUser(std::string hostOrJoin);

int main()
{
	// Initialize all main objects
	ClientThread client;
	ServerThread server;
	KeyboardThread keyboard;
	ViewThread view;

	// Declare a temp and serverThread, which will be used later
	std::string hostOrJoin;
	std::thread *serverThread;
	
	Global::cursor.setColor(Color::WHITE);

	if (isServer())
	{
		hostOrJoin = "host";
		std::cout << std::endl << "Starting server..." << std::endl;

		// Attempt to initialize the server, then start it's acceping thread for clients to connect
		server.init();
		serverThread = new std::thread(&ServerThread::start, server);

		// Hooray!
		std::cout << "Server successfully initialized!" << std::endl;

		// Initialize local client
		client.init("127.0.0.1");
		std::cout << std::endl << "Connected to local server" << std::endl;
		std::cout << "Make sure port " << Global::PORT << " is open to host properly!" << std::endl;
	}
	else
	{
		hostOrJoin = "join";
		std::string input;
		do
		{
			// Get the hostname
			std::cout << std::endl << "What's the host name of the server?" << std::endl;
			getline(std::cin, input);
			std::cout << std::endl << "Connecting to server..." << std::endl;

			// Attempt to connect
		} while (client.init(input) != 0);

		std::cout << "Successfully joined server!" << std::endl;
	}

	// Get user info
	initUser(hostOrJoin);

	// Clear the screen so it's ready for chatting
	system(Global::CLEAR.c_str());

	// Start each different thread in their own loops
	std::thread clientThread(&ClientThread::start, client);
	std::thread viewThread(&ViewThread::start, view);
	keyboard.start();
	// The program is now running

	// Wait until the keyboard thread stops

	// Stop all threads
	client.stop();
	server.stop();
	view.stop();
	clientThread.join();
	viewThread.join();

	return 0;
}

bool isServer()
{
	bool valid = false;
	std::string input = "";

	std::cout << std::endl << "\t\t\t   Welcome to General Chat!" << std::endl;
	std::cout << "To start, I'll need to know if you're hosting a chatroom, or joining one." << std::endl;
	std::cout << std::endl << "Do you want to Host or Join a chatroom? ";

	while (!valid)
	{
		// Get input, then make it ignore case
		getline(std::cin, input);
		std::transform(input.begin(), input.end(), input.begin(), tolower);

		// Check if it's valid input. If it is, return if this program will be the server
		if (input == "host")
		{
			return true;
		}
		else if (input == "join")
		{
			return false;
		}
		else
		{
			std::cout << "I didn't get that, please enter 'host' or 'join': ";
		}
	}

	return false; // Just in case valid doesn't get initialized properly
}

void initUser(std::string hostOrJoin)
{
	bool valid = false;
	std::string input;

	std::cout << std::endl << "What's your username? ";

	while (!valid)
	{
		// Get the username, then check if it's too long or too short
		getline(std::cin, input);
		if (input.length() > Global::MAX_USERNAME)
		{
			std::cout << "The name you entered was too long!" << std::endl;
			std::cout << "Please enter a username (20 characters max): ";
		}
		else if (input.length() < 1)
		{
			std::cout << "The name you entered was too short!" << std::endl;
			std::cout << "Please enter a username (20 characters max): ";
		}
		else
		{
			valid = true;
		}
	}

	// Set the username
	Global::username = input;
	valid = false;

	// Print out the color list
	std::cout << std::endl << "Choose a color (";
	std::map<char, Color>::const_iterator it;
	for (it = Global::colorMap.begin(); it != Global::colorMap.end(); it++)
	{
		if (it->second != Color::WHITE)
		{
			Global::cursor.setColor(it->second);
			std::cout << it->first;

			// Only separate by commas if it isnt last
			std::map<char, Color>::const_iterator temp = it;
			if (++temp != Global::colorMap.end())
			{
				Global::cursor.setColor(Color::WHITE);
				std::cout << ", ";
			}
		}
	}
	// Reset the color and finish the sentence
	Global::cursor.setColor(Color::WHITE);
	std::cout << ") ";

	while (!valid)
	{
		// Ask for a color until a valid one is given
		getline(std::cin, input);
		std::transform(input.begin(), input.end(), input.begin(), tolower);

		if (input == "red" || input == "r")
		{
			Global::color = Color::RED;
			valid = true;
		}
		else if (input == "yellow" || input == "y")
		{
			Global::color = Color::YELLOW;
			valid = true;
		}
		else if (input == "green" || input == "g")
		{
			Global::color = Color::GREEN;
			valid = true;
		}
		else if (input == "cyan" || input == "c")
		{
			Global::color = Color::CYAN;
			valid = true;
		}
		else if (input == "blue" || input == "b")
		{
			Global::color = Color::BLUE;
			valid = true;
		}
		else if (input == "magenta" || input == "m")
		{
			Global::color = Color::MAGENTA;
			valid = true;
		}
		else
		{
			std::cout << "I didn't get that, please enter a valid color: ";
		}
	}
	
	// Finish printing out to user and wait for confirmation
	std::cout << std::endl << "Welcome ";
	Global::cursor.setColor(Global::color);
	std::cout << Global::username;
	Global::cursor.setColor(Color::WHITE);
	std::cout << "!" << std::endl;
	std::cout << "Press a key when ready to " << hostOrJoin << "..." << std::endl;
	_getch();

	// Send out a message to everyone connected that someone logged on
	std::string temp = Global::username + " entered chat";
	FullMessage *message = new FullMessage("Server", temp, Color::WHITE);
	message->type = Notification::NEW_LOCAL_MESSAGE;
	Global::messageQueue.push(message);
}