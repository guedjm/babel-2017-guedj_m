#pragma once

# include <string>

class ITCPSocket
{
public:
	virtual ~ITCPSocket() {}

	virtual int 	init() = 0;

	virtual int		getFd() const = 0;

	virtual int		connectTo(std::string const &, short) = 0;
	virtual int		bindSock(std::string const &, short) = 0;
	virtual int		listenSock(int) = 0;
	virtual int		closeSock() = 0;

	virtual int		receive(std::string &) = 0;
	virtual int		sendData(std::string const &, int) = 0;

	virtual int		acceptClient(struct sockaddr_in &) = 0;
};
