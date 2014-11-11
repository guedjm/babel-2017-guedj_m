#ifdef _WIN32

#include "WindowsTCPClient.h"
#include "WindowsTCPSocket.h"

WindowsTCPClient::WindowsTCPClient()
{
	this->_toSendLen = 0;
	this->_sock = new WindowsTCPSocket();
	if (this->_sock == 0 || this->_sock->init() == -1)
		throw std::exception();
}

WindowsTCPClient::~WindowsTCPClient()
{
	delete this->_sock;
}

int 			WindowsTCPClient::closeSock()
{
	return (this->_sock->closeSock());
}

int 			WindowsTCPClient::getWriteFd() const
{
	return (this->_sock->getFd());
}

int 			WindowsTCPClient::getReadFd() const
{
	return (this->_sock->getFd());
}

bool			WindowsTCPClient::somethingToWrite() const
{
	return (this->_toSendLen != 0);
}

int 			WindowsTCPClient::connectTo(std::string const &ip, short port)
{
	int 		ret;

	ret = this->_sock->connectTo(ip, port);
	return (ret);
}

int 			WindowsTCPClient::operator>>(std::string &msg)
{
	int 		ret;

	ret = this->_sock->receive(msg);
	return (ret);
}

void 			WindowsTCPClient::prepareMsg(std::string const &msg, int len)
{
	this->_toSend += msg;
	this->_toSendLen += len;
}

int 			WindowsTCPClient::send()
{
	int 	ret;

	ret = this->_sock->sendData(this->_toSend, this->_toSendLen);
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