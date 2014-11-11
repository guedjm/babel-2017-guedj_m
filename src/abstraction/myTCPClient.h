#pragma once

#include "LinuxTCPClient.h"
#include "WindowsTCPClient.h"

class myTCPClient :
#ifdef _WIN32
	public WindowsTCPClient
#else
	public LinuxTCPClient
#endif
{
public:
	myTCPClient(){};
	~myTCPClient(){};
};