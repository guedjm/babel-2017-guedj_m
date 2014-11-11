#ifndef _WIN32

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "LinuxTCPSocket.h"

LinuxTCPSocket::LinuxTCPSocket()
{
	memset(&this->_addr, 0, sizeof(this->_addr));
}

LinuxTCPSocket::LinuxTCPSocket(struct sockaddr_in &addr, int fd)
{
	this->_fd = fd;
	memcpy(&this->_addr, &addr, sizeof(addr));
}

LinuxTCPSocket::~LinuxTCPSocket()
{
}

int			LinuxTCPSocket::init()
{
	int 	ret;

	ret = socket(AF_INET, SOCK_STREAM, 0);
	if (ret != -1)
		this->_fd = ret;
	return (ret);
}

int 		LinuxTCPSocket::closeSock()
{
	return (close(this->_fd));
}

int 		LinuxTCPSocket::getFd() const
{
	return (this->_fd);
}

int 					LinuxTCPSocket::connectTo(std::string const &ip, short port)
{
	int 				ret;

	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	ret = connect(this->_fd, (struct sockaddr*)(&this->_addr),
		sizeof(this->_addr));
	return (ret);
}

int 					LinuxTCPSocket::bindSock(std::string const &ip, short port)
{
	int 				ret;

	this->_addr.sin_family = AF_INET;
	if (ip == "")
		this->_addr.sin_addr.s_addr = INADDR_ANY;
	else
		this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	if (port != 0)
		this->_addr.sin_port = htons(port);
	ret = bind(this->_fd, (struct sockaddr*)(&this->_addr),
		sizeof(this->_addr));
	return (ret);
}

int 					LinuxTCPSocket::listenSock(int backlog)
{
	int 				ret;

	ret = listen(this->_fd, backlog);
	return (ret);
}

int 					LinuxTCPSocket::acceptClient(struct sockaddr_in &addr)
{
	int 				ret;
	int 				len;

	len = sizeof(addr);
	ret = accept(this->_fd, (struct sockaddr*)(&addr),
		(socklen_t*)&len);
	return (ret);
}

int 				LinuxTCPSocket::receive(std::string &buff)
{
	int 			i;
	int 			totalSize;
	char			tmp[1500];

	totalSize = 0;
	while (totalSize == 0 || i == 1500)
	{
		i = read(this->_fd, tmp, 1500);
		if (i <= 0)
			return (i);
		buff.append(tmp, i);
		totalSize += i;
	}
	return (totalSize);
}

int 				LinuxTCPSocket::sendData(std::string const &buff,
	int len)
{
	int 			ret;

	ret = write(this->_fd, buff.c_str(), len);
	return (ret);
}
#endif