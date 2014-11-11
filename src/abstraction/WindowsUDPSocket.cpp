#ifdef _WIN32

#include <string.h>
#include <iostream>
#include <stdio.h>
#include "WindowsUDPSocket.h"

WindowsUDPSocket::WindowsUDPSocket()
{
	memset(&this->_addr, 0, sizeof(this->_addr));
}

WindowsUDPSocket::~WindowsUDPSocket()
{
}

int		WindowsUDPSocket::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &this->_wsaData) != 0)
	{
		printf("Initialization failed with error code : %d\n", WSAGetLastError());
		return (-1);
	}
	this->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (this->_socket == INVALID_SOCKET)
	{
		printf("Could not create socket : %d\n", WSAGetLastError());
		return (-1);
	}
	this->_fd = this->_socket;
	return (this->_fd);
}

int		WindowsUDPSocket::closeSock()
{
	return (closesocket(this->_fd));
}

int		WindowsUDPSocket::getFd() const
{
	return (this->_socket);
}

unsigned short	WindowsUDPSocket::getPort() const
{
	return (this->_port);
}

int		WindowsUDPSocket::bindSock(std::string const &ip, unsigned short &port)
{
	int	ret;

	this->_addr.sin_family = AF_INET;
	if (ip == "")
		this->_addr.sin_addr.s_addr = INADDR_ANY;
	else
		this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	this->_addr.sin_port = htons(port);
	ret = bind(this->_socket, (struct sockaddr*)(&this->_addr), sizeof(this->_addr));
	if (ret == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d\n", WSAGetLastError());
		return (-1);
	}
	else
	{
		this->_port = ntohs(this->_addr.sin_port);
		port = this->_port;
	}
	return (ret);
}

int		WindowsUDPSocket::recvFrom(char *buff, int size, std::string &ip, unsigned short &port)
{
	int	ret;
	int	len = sizeof(this->_addr);

	ret = recvfrom(this->_socket, buff, size, 0, (struct sockaddr*)(&this->_addr), &len);
	if (ret != -1)
	{
		ip = inet_ntoa(this->_addr.sin_addr);
		port = ntohs(this->_addr.sin_port);
	}
	return (ret);
}

int		WindowsUDPSocket::sendTo(const char *buff, int size, std::string const &ip, unsigned short port)
{
	int	ret;

	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	ret = sendto(this->_socket, buff, size, 0, (struct sockaddr*)(&this->_addr), sizeof(this->_addr));
	return (ret);
}

#endif