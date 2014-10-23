/*
MessageQueue is the core of this program's threading structure.
It allows the 3 main threads to access it without any conflict
by using a mutex and condition variables. This variable locks
when more than 1 thread is trying to access it.

Think of this class as the funnel for the 3 threads.

KeyboardThread and ClientThread will be occasionally pushing
data onto the MessageQueue.

ViewThread will be constantly popping off of the queue. If there
isn't anything in the queue, it'll wait until someone else pushes
onto it.

This communicates closely with KeyboardThread, ClientThread, 
and ViewThread
*/

#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE

#include "stdafx.h"
#include "Message.h"

class MessageQueue
{
private:
	std::mutex lock;
	std::condition_variable notifier;
	bool ready;
	std::queue<Message*> buffer;

public:
	MessageQueue();
	void push(Message *message);
	Message* pop();
	int getSize()
	{
		return buffer.size();
	}
};

#endif