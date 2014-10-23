// All standard includes needed for Windows systems

#ifndef GC_WINDOWS
#define GC_WINDOWS

// Remove winsock from windows.h, then include winsock2.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <conio.h>
#include <winsock2.h>
#include <io.h>

// Compile for Visual Studio
#pragma comment(lib, "Ws2_32.lib")

#endif