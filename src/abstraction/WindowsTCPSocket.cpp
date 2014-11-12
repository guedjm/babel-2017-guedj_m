#ifdef _WIN32

#include "WindowsTCPSocket.h"
#include "constant.h"

WindowsTCPSocket::WindowsTCPSocket()
{
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_result = NULL;
	this->_clientSocket = INVALID_SOCKET;
}

WindowsTCPSocket::WindowsTCPSocket(struct sockaddr_in &addr, int fd)
{
	this->_fd = fd;
	this->_clientSocket = fd;
	memcpy(&this->_addr, &addr, sizeof(addr));
}

WindowsTCPSocket::~WindowsTCPSocket()
{
}

int			WindowsTCPSocket::init()
{
	int 	ret;

	ret = WSAStartup(MAKEWORD(2, 2), &this->_wsaData);
	if (ret != 0)
		throw std::runtime_error("WSAStartup failed");

	ZeroMemory(&this->_hints, sizeof(this->_hints));
	this->_hints.ai_family = AF_INET;
	this->_hints.ai_socktype = SOCK_STREAM;
	this->_hints.ai_protocol = IPPROTO_TCP;
	this->_hints.ai_flags = AI_PASSIVE;

	ret = getaddrinfo(NULL, DEFAULT_PORT, &this->_hints, &this->_result);
	if (ret != 0) {
		WSACleanup();
		throw std::runtime_error("getaddrinfo failed");
	}
	this->_clientSocket = socket(this->_result->ai_family, this->_result->ai_socktype, this->_result->ai_protocol);
	if (this->_clientSocket == INVALID_SOCKET)
	{
		freeaddrinfo(this->_result);
		WSACleanup();
		throw std::runtime_error("socket failed");
	}
	else
		this->_fd = this->_clientSocket;
	return (this->_clientSocket);
}

int 		WindowsTCPSocket::closeSock()
{
	closesocket(this->_clientSocket);
	WSACleanup();
	return (1);
}

int 		WindowsTCPSocket::getFd() const
{
	return (this->_fd);
}

int 					WindowsTCPSocket::connectTo(std::string const &ip, short port)
{
	int 				ret;

	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	ret = connect(this->_clientSocket, (sockaddr *)(&this->_addr), sizeof(this->_addr));

	return (ret);
}

int 					WindowsTCPSocket::bindSock(std::string const &ip, short port)
{
	int iResult;

	this->_addr.sin_family = AF_INET;
	if (ip == "")
		this->_addr.sin_addr.s_addr = INADDR_ANY;
	else
		this->_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	if (port != 0)
		this->_addr.sin_port = htons(port);
	iResult = bind(this->_clientSocket, (sockaddr *)(&this->_addr), sizeof(this->_addr));
	if (iResult == SOCKET_ERROR)
	{
		closesocket(this->_clientSocket);
		WSACleanup();
		throw std::runtime_error("bind failed");
	}
	return (1);
}

int 					WindowsTCPSocket::listenSock(int backlog)
{
	int 				ret;

	ret = listen(this->_clientSocket, backlog);
	if (ret == SOCKET_ERROR)
	{
		closesocket(this->_clientSocket);
		WSACleanup();
		throw std::runtime_error("listen failed");
	}
	return (0);
}

int 					WindowsTCPSocket::acceptClient(struct sockaddr_in &addr_in)
{
	int					remoteFd;

	socklen_t in_len = sizeof(addr_in);
	remoteFd = accept(this->_clientSocket, (sockaddr *)&addr_in, &in_len);
	if (remoteFd == INVALID_SOCKET)
	{
		closesocket(remoteFd);
		WSACleanup();
		throw std::runtime_error("accept failed");
	}
	return (remoteFd);
}

int 				WindowsTCPSocket::receive(std::string &buff)
{
	int 			i;
	int 			totalSize;
	char			tmp[BUFFER_SIZE];

	totalSize = 0;
	while (totalSize == 0 || i == BUFFER_SIZE)
	{
		i = recv(this->_clientSocket, tmp, BUFFER_SIZE, 0);
		if (i <= 0)
			return (i);
		buff.append(tmp, i);
		totalSize += i;
	}
	return (totalSize);
}

int 				WindowsTCPSocket::sendData(std::string const &buff,
	int len)
{
	int		iSendResult;

 	iSendResult = send(this->_clientSocket, buff.c_str(), len, 0);
	if (iSendResult == SOCKET_ERROR)
	{
		closesocket(this->_clientSocket);
		WSACleanup();
		throw std::runtime_error("send failed");
	}
	return (iSendResult);
}

#endif