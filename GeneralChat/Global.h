// Declarations of all global variables go in this namespace

#ifndef GLOBAL
#define GLOBAL

#include "stdafx.h"
#include "Color.h"
#include "KeyboardBuffer.h"
#include "MessageHistory.h"
#include "MessageQueue.h"
#include "Client.h"
#include "Server.h"
#include "Cursor.h"

namespace Global
{
	// Global boolean used to determing if the program is still running
	extern std::atomic<bool> running;

	// Mutex used to lock output
	extern std::mutex ostreamLock;
	extern std::mutex serverLock;

	// Thread used for server polling
	extern std::thread *acceptThread;

	// Standard Message containers
	extern KeyboardBuffer keyboardBuffer;
	extern MessageHistory messageHistory;
	extern MessageQueue messageQueue;
	extern Client client;
	extern Server server;
	extern Cursor cursor;

	// This user's username
	extern std::string username;
	extern Color color;

	// Color map used by the Formatter
	const std::map<char, Color> colorMap = {
		std::pair<char, Color>('W', Color::WHITE),
		std::pair<char, Color>('R', Color::RED),
		std::pair<char, Color>('Y', Color::YELLOW),
		std::pair<char, Color>('G', Color::GREEN),
		std::pair<char, Color>('C', Color::CYAN),
		std::pair<char, Color>('B', Color::BLUE),
		std::pair<char, Color>('M', Color::MAGENTA),
	};

	// Constants
	const int CONSOLE_WIDTH = 80;
	const int PORT = 8888;
	const std::string DIVIDER = ": "; // Divides message as - username: message
	// <Color> Username <Delim> Message <\n\0>
	const int MAX_USERNAME = 20;
	const int MAX_MESSAGE = 200;
	const int MAX_LENGTH = MAX_USERNAME + MAX_MESSAGE + 4;

	#ifdef _WIN32
	const int BUFFER_HEIGHT = 300;
	const std::string CLEAR = "cls";
	const int ENTER = 13;
	const int BACKSPACE = 8;
	const int ESC = 27;
	#else // Unix
	const int BUFFER_HEIGHT = 24;
	const std::string CLEAR = "tput reset";
	const int ENTER = 10;
	const int BACKSPACE = 127; // DEL == 127 under MS-DOS
	const int ESC = 27;
	#endif
}
#endif