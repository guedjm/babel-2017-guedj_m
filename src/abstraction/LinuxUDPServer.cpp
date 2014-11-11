#ifndef _WIN32

#include <exception>
#include "LinuxUDPServer.h"
#include "LinuxUDPSocket.h"

LinuxUDPServer::LinuxUDPServer()
{
    this->_sock = new LinuxUDPSocket();
    if (this->_sock == 0 || this->_sock->init() == -1)
        throw std::exception();
}

LinuxUDPServer::~LinuxUDPServer()
{
    delete this->_sock;
}

int             LinuxUDPServer::getReadFd() const
{
    return (this->_sock->getFd());
}

int             LinuxUDPServer::start(std::string const &ip, unsigned short &port)
{
    return (this->_sock->bindSock(ip, port));
}

void             LinuxUDPServer::stop()
{
    this->_sock->closeSock();
}

int             LinuxUDPServer::recvFrom(char *buff, int size, std::string &ip, unsigned short &port)
{
    return (this->_sock->recvFrom(buff, size, ip, port));
}

int             LinuxUDPServer::sendTo(const char *buff, int size, std::string const &ip, unsigned short port)
{
    return (this->_sock->sendTo(buff, size, ip, port));
}
#endif
