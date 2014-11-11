#pragma once

#include "WindowsTCPSocket.h"
#include "LinuxTCPSocket.h"

class myTCPSocket :
#ifdef _WIN32
	public WindowsTCPSocket
#else
	public LinuxTCPSocket
#endif
{
public:
	myTCPSocket(){};
	~myTCPSocket(){};
};