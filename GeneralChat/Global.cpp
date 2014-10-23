#include "Global.h"

// Global boolean used to determing if the program is still running
std::atomic<bool> Global::running;

// Mutex used to lock output
std::mutex Global::ostreamLock;
std::mutex Global::serverLock;

// Thread used for server polling
std::thread *Global::acceptThread;

// Standard Message containers
KeyboardBuffer Global::keyboardBuffer;
MessageHistory Global::messageHistory;
MessageQueue Global::messageQueue;
Client Global::client;
Server Global::server;
Cursor Global::cursor;

// This user's username
std::string Global::username;
Color Global::color;