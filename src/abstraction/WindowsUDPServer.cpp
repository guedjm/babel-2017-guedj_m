#ifdef _WIN32

#include "WindowsUDPServer.h"
#include <exception>
#include "WindowsUDPSocket.h"

WindowsUDPServer::WindowsUDPServer()
{
	this->_sock = new WindowsUDPSocket();
	if (this->_sock == 0 || this->_sock->init() == -1)
		throw std::exception();
}

WindowsUDPServer::~WindowsUDPServer()
{
	delete this->_sock;
}

int             WindowsUDPServer::getReadFd() const
{
	return (this->_sock->getFd());
}

int             WindowsUDPServer::start(std::string const &ip, unsigned short &port)
{
	return (this->_sock->bindSock(ip, port));
}

void             WindowsUDPServer::stop()
{
	this->_sock->closeSock();
}

int             WindowsUDPServer::recvFrom(char *buff, int size, std::string &ip, unsigned short &port)
{
	return (this->_sock->recvFrom(buff, size, ip, port));
}

int             WindowsUDPServer::sendTo(const char *buff, int size, std::string const &ip, unsigned short port)
{
	return (this->_sock->sendTo(buff, size, ip, port));
}

#endif