#ifndef _WIN32

#pragma once

#include "IUDPSocket.h"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


class LinuxUDPSocket : public IUDPSocket
{
public:
	LinuxUDPSocket();
	virtual ~LinuxUDPSocket();

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
};

#endif