// Simple ifdef for cross platform includes

#ifndef OS
#define OS

#ifdef _WIN32
#include "GC_Windows.h"
#else
#include "GC_Unix.h"
#endif
#endif