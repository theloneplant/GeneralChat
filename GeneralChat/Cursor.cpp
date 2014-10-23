#include "Cursor.h"
#include "Global.h"

// Windows version
#ifdef _WIN32
#include "GC_Windows.h"
void Cursor::setColor(Color color)
{
	// Use the windows handle to change the screen color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Cursor::setPosition(int x, int y)
{
	// Use the windows handle to change the cursor position
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// Unix version
#else
#include "GC_Unix.h"
void Cursor::setColor(Color color)
{
	// Output an ANSI escape code depending on color
	switch (color)
	{
	case BLUE:
		std::cout << "\033[34m";
		break;
	case GREEN:
		std::cout << "\033[32m";
		break;
	case CYAN:
		std::cout << "\033[36m";
		break;
	case RED:
		std::cout << "\033[31m";
		break;
	case MAGENTA:
		std::cout << "\033[35m";
		break;
	case YELLOW:
		std::cout << "\033[33m";
		break;
	case WHITE:
		std::cout << "\033[37m";
		break;
	}
}

void Cursor::setPosition(int x, int y)
{
	// Set the cursor position with an ANSI escape code
	std::cout << "\033[" << y + 1 << ";" << x + 1 << "f";
}
#endif