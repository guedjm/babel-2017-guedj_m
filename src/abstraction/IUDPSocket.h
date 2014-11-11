#pragma once

#include <string>

class IUDPSocket
{
public:
	virtual ~IUDPSocket() {}

	virtual int		init() = 0;

	virtual int		getFd() const = 0;
	virtual unsigned short	getPort() const = 0;

	virtual int		bindSock(std::string const &, unsigned short &) = 0;
	virtual int		closeSock() = 0;

	virtual int		recvFrom(char *, int, std::string &, unsigned short &) = 0;
	virtual int		sendTo(const char *, int, std::string const &, unsigned short) = 0;

};
