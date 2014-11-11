#pragma once
#ifdef _WIN32

#include "IUDPSocket.h"
#include <string>
# include <winsock2.h>
# include <Windows.h>


class WindowsUDPSocket : public IUDPSocket
{
public:
	WindowsUDPSocket();
	virtual ~WindowsUDPSocket();

	virtual int		init();
	virtual int		closeSock();

	virtual int		getFd() const;
	virtual unsigned short	getPort() const;

	virtual int		bindSock(std::string const &, unsigned short &);

	virtual int		recvFrom(char *, int, std::string &, unsigned short &);
	virtual int		sendTo(const char *, int, std::string const &, unsigned short);

private:
	int					_fd;
	unsigned short		_port;
	struct sockaddr_in	_addr;
	WSADATA				_wsaData;
	SOCKET				_socket;
};

#endif