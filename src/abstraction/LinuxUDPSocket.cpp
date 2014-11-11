#ifndef _WIN32

#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include "LinuxUDPSocket.h"

LinuxUDPSocket::LinuxUDPSocket()
{
	memset(&this->_addr, 0, sizeof(this->_addr));
}

LinuxUDPSocket::~LinuxUDPSocket()
{
}

int		LinuxUDPSocket::init()
{
	int ret;

	ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ret != -1)
		this->_fd = ret;
	return (ret);
}

int		LinuxUDPSocket::closeSock()
{
	return (close(this->_fd));
}

int		LinuxUDPSocket::getFd() const
{
	return (this->_fd);
}

unsigned short	LinuxUDPSocket::getPort() const
{
	return (this->_port);
}

int		LinuxUDPSocket::bindSock(std::string const &ip, unsigned short &port)
{
	int	ret;

	this->_addr.sin_family = AF_INET;
	if (ip == "")
		this->_addr.sin_addr.s_addr = INADDR_ANY;
	else
		this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	this->_addr.sin_port = htons(port);
	ret = bind(this->_fd, (struct sockaddr*)(&this->_addr), sizeof(this->_addr));
	if (ret != -1)
	{
		this->_port = ntohs(this->_addr.sin_port);
		port = this->_port;
	}
	return (ret);
}

int		LinuxUDPSocket::recvFrom(char *buff, int size, std::string &ip, unsigned short &port)
{
	int	ret;
	int	len = sizeof(this->_addr);

	ret = recvfrom(this->_fd, buff, size, 0, (struct sockaddr*)(&this->_addr), (socklen_t*)(&len));
	if (ret != -1)
	{
		ip = inet_ntoa(this->_addr.sin_addr);
        port = ntohs(this->_addr.sin_port);
    }
	return (ret);
}

int		LinuxUDPSocket::sendTo(const char *buff, int size, std::string const &ip, unsigned short port)
{
	int	ret;

	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	ret = sendto(this->_fd, buff, size, 0, (struct sockaddr*)(&this->_addr), sizeof(this->_addr));
	return (ret);
}

#endif