#ifndef _WIN32

#pragma once

# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "ITCPSocket.h"

class LinuxTCPSocket : public ITCPSocket
{
public:
	LinuxTCPSocket();
	LinuxTCPSocket(struct sockaddr_in &, int);
	virtual ~LinuxTCPSocket();

	virtual int		init();
	virtual int		closeSock();

	virtual int 	getFd() const;

	virtual int		connectTo(std::string const &, short);

	virtual int		bindSock(std::string const & = "", short = 0);
	virtual int		listenSock(int);
	virtual int 	acceptClient(struct sockaddr_in &);

	virtual int		receive(std::string &);
	virtual int		sendData(std::string const &, int);

private:
	int 				_fd;
	struct sockaddr_in 	_addr;
};
#endif