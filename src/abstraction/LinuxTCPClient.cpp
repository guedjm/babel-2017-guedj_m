#ifndef _WIN32

#include "LinuxTCPClient.h"
#include "LinuxTCPSocket.h"

LinuxTCPClient::LinuxTCPClient()
{
	this->_toSendLen = 0;
	this->_sock = new LinuxTCPSocket();
	if (this->_sock == 0 || this->_sock->init() == -1)
		throw std::exception();
}

LinuxTCPClient::~LinuxTCPClient()
{
	delete this->_sock;
}

int 			LinuxTCPClient::closeSock()
{
	return (this->_sock->closeSock());
}

int 			LinuxTCPClient::getWriteFd() const
{
	return (this->_sock->getFd());
}

int 			LinuxTCPClient::getReadFd() const
{
	return (this->_sock->getFd());
}

bool			LinuxTCPClient::somethingToWrite() const
{
	return (this->_toSendLen != 0);
}

int 			LinuxTCPClient::connectTo(std::string const &ip, short port)
{
	int 		ret;

	ret = this->_sock->connectTo(ip, port);
	return (ret);
}

int 			LinuxTCPClient::operator>>(std::string &msg)
{
	int 		ret;

	ret = this->_sock->recv(msg);
	return (ret);
}

void 			LinuxTCPClient::prepareMsg(std::string const &, int)
{
	//TODO
}

int 			LinuxTCPClient::send()
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