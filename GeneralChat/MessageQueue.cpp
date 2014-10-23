#include "Global.h"
#include "MessageQueue.h"

MessageQueue::MessageQueue()
{
	// When something's in the queue, ready is true
	ready = false;
}

void MessageQueue::push(Message *message)
{
	// Wait until another thread is done
	std::lock_guard<std::mutex> waitLock(lock);

	buffer.push(message);

	// Trigger anyone waiting on the thread
	ready = true;
	notifier.notify_one();
}

Message* MessageQueue::pop()
{
	// Wait until another thread is done
	std::unique_lock<std::mutex> waitLock(lock);
	while (!ready)
	{
		// Sleep the thread until available
		notifier.wait(waitLock);
	}

	// Pop the most current message off the buffer; this will be deleted later
	Message *message = buffer.front();
	buffer.pop();

	// Double check the buffer size
	if (buffer.size() > 0)
		ready = true;
	else
		ready = false;

	return message;
}