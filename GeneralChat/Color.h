/*
Color enum used to determine which color
the username is. Values here are for Windows,
and values for linux are set at Cursor.

This communicates closely with Cursor.
*/

#ifndef COLOR
#define COLOR

#include "stdafx.h"

enum Color
{
	BLUE = 0x09,
	GREEN = 0x0A,
	CYAN = 0x0B,
	RED = 0x0C,
	MAGENTA = 0x0D,
	YELLOW = 0x0E,
	WHITE = 0x0F
};
#endif