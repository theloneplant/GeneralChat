/*
Notification is an enum that determines which
type of message was received.

This communicates closely with ViewThread
*/

#ifndef NOTIFICATION
#define NOTIFICATION

enum Notification
{
	NEW_MESSAGE,
	NEW_LOCAL_MESSAGE,
	NEW_CHAR
};

#endif