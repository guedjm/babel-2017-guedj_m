#ifndef _WIN32

#include <exception>
#include "LinuxUDPClient.h"
#include "LinuxUDPSocket.h"

LinuxUDPClient::LinuxUDPClient()
{
    this->_sock = new LinuxUDPSocket();
    if (this->_sock == 0 || this->_sock->init() == -1)
        throw  std::exception();
}

LinuxUDPClient::~LinuxUDPClient()
{
    delete this->_sock;
}

int         LinuxUDPClient::getReadFd() const
{
    return (this->_sock->getFd());
}

void        LinuxUDPClient::closeSock()
{
    this->_sock->closeSock();
}

int         LinuxUDPClient::recvFrom(char *buff, int size, std::string &ip, unsigned short &port)
{
    return (this->_sock->recvFrom(buff, size, ip, port));
}

int         LinuxUDPClient::sendTo(const char *buff, int size, std::string const &ip, unsigned short port)
{
    return (this->_sock->sendTo(buff, size, ip, port));
}
#endif
