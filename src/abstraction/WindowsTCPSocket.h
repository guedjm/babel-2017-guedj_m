#pragma once
#ifdef _WIN32

# include <winsock2.h>
# include <windows.h>
# include <ws2tcpip.h>
# include <stdlib.h>
# include <stdio.h>
# pragma comment(lib, "Ws2_32.lib")

# include "constant.h"
# include "ITCPSocket.h"


class WindowsTCPSocket : public ITCPSocket
{
public:
	WindowsTCPSocket();
	WindowsTCPSocket(struct sockaddr_in &, int);
	virtual ~WindowsTCPSocket();

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
	SOCKET				_clientSocket;
	WSADATA				_wsaData;
	SOCKADDR_IN		 	_addr;
	struct addrinfo		*_result;
	struct addrinfo		_hints;
};

#endif