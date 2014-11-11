#pragma once
#ifdef _WIN32

#include <string>
#include "IUDPSocket.h"
#include "IUDPServer.h"

class WindowsUDPServer : public IUDPServer
{
public:
	WindowsUDPServer();
	virtual ~WindowsUDPServer();

	virtual int     getReadFd() const;

	virtual int         start(std::string const &ip, unsigned short &port);
	virtual void        stop();

	virtual int         recvFrom(char *buff, int size, std::string &ip, unsigned short &port);
	virtual int         sendTo(const char *buff, int size, std::string const &ip, unsigned short port);

private:
	IUDPSocket  *_sock;
};

#endif