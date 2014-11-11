#ifndef _WIN32

#include <exception>
#include "LinuxTCPServer.h"
#include "LinuxTCPSocket.h"
#include "LinuxTCPRemoteClient.h"

LinuxTCPServer::LinuxTCPServer()
{
	this->_sock = new LinuxTCPSocket();
	if (this->_sock == 0 || this->_sock->init() == -1)
		throw std::exception();
}

LinuxTCPServer::~LinuxTCPServer()
{
	delete this->_sock;
}

int 			LinuxTCPServer::getReadFd() const
{
	return (this->_sock->getFd());
}

int 			LinuxTCPServer::start(std::string const &ip,
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

void			LinuxTCPServer::stop()
{
	this->_sock->closeSock();
}

ITCPRemoteClient		*LinuxTCPServer::acceptClient()
{
	int 				ret;
	struct sockaddr_in	addr;
	ITCPRemoteClient 		*newClient;


	ret = this->_sock->acceptClient(addr);
	if (ret == -1)
		throw std::exception();
	newClient = new LinuxTCPRemoteClient(addr, ret);
	return (newClient);
}
#endif