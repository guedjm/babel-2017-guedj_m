#pragma once

#include "LinuxTCPServer.h"
#include "WindowsTCPServer.h"

class myTCPServer :
#ifdef _WIN32
	public WindowsTCPServer
#else
	public LinuxTCPServer
#endif
{
public:
	myTCPServer(){};
	~myTCPServer(){};
};