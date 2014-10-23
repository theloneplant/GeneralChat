#ifndef CONIO_UNIX
#define CONIO_UNIX

#include "stdafx.h"
#include "OS.h"
#include "Global.h"

static char _getch()
{
	return getchar();
}

static void conioInit()
{
	// Set the terminal to be unbuffered
	struct termios newt;
	tcgetattr(STDIN_FILENO, &newt);
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

#endif