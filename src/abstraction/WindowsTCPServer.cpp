#ifdef _WIN32

#include <exception>
#include "WindowsTCPServer.h"
#include "WindowsTCPSocket.h"
#include "WindowsTCPRemoteClient.h"

WindowsTCPServer::WindowsTCPServer()
{
	this->_sock = new WindowsTCPSocket();
	if (this->_sock == 0 || this->_sock->init() == -1)
		throw std::exception();
}

WindowsTCPServer::~WindowsTCPServer()
{
	delete this->_sock;
}

int 			WindowsTCPServer::getReadFd() const
{
	return (this->_sock->getFd());
}

int 			WindowsTCPServer::start(std::string const &ip,
	short port, int backlog)
{
	int 		ret;

	ret = this->_sock->bindSock(ip, port);
	if (ret != -1)
		ret = this->_sock->listenSock(backlog);
	if (ret == -1)
		throw std::exception();
	return (ret);
}

void			WindowsTCPServer::stop()
{
	this->_sock->closeSock();
}

ITCPRemoteClient		*WindowsTCPServer::acceptClient()
{
	int 				ret;
	struct sockaddr_in	addr;
	ITCPRemoteClient 		*newClient;


	ret = this->_sock->acceptClient(addr);
	if (ret == -1)
		throw std::exception();
	newClient = new WindowsTCPRemoteClient(addr, ret);
	return (newClient);
}

#endif