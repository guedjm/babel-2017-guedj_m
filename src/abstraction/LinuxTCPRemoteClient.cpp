#ifndef _WIN32

#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "LinuxTCPSocket.h"
#include "LinuxTCPRemoteClient.h"

LinuxTCPRemoteClient::LinuxTCPRemoteClient(struct sockaddr_in &addr,
	int fd)
{
	this->_closing = false;
	this->_sock = new LinuxTCPSocket(addr, fd);
	if (this->_sock == 0)
		throw std::exception();
	this->_ip = inet_ntoa(addr.sin_addr);
	this->_toSendLen = 0;
}

LinuxTCPRemoteClient::~LinuxTCPRemoteClient()
{
	this->_sock->closeSock();
	delete this->_sock;
}

int 		LinuxTCPRemoteClient::getWriteFd() const
{
	return (this->_sock->getFd());
}

int			LinuxTCPRemoteClient::getReadFd() const
{
	return (this->_sock->getFd());
}

std::string const 	&LinuxTCPRemoteClient::getIp() const
{
	return (this->_ip);
}

void				LinuxTCPRemoteClient::prepareClose()
{
	this->_closing = true;
}

bool				LinuxTCPRemoteClient::mustBeClosed() const
{
	return (this->_closing);
}

bool 		LinuxTCPRemoteClient::somethingToWrite() const
{
	return (this->_toSendLen != 0);
}

int 		LinuxTCPRemoteClient::operator>>(std::string &data)
{
	int		ret;

	ret = this->_sock->recv(data);
	if (ret == -1)
		throw std::exception();
	return (ret);
}

void 		LinuxTCPRemoteClient::prepareMsg(std::string const &, int)
{
	//TODO
}

int 		LinuxTCPRemoteClient::send()
{
	int 	ret;

	ret = this->_sock->send(this->_toSend, this->_toSendLen);
	if (ret == -1)
		throw std::exception();
	if (ret != this->_toSendLen)
	{
		this->_toSend = this->_toSend.substr(ret);
		this->_toSendLen -= ret;
	}
	else
	{
		this->_toSend.clear();
		this->_toSendLen = 0;
	}
	return (ret);
}
#endif