#pragma once
#ifdef _WIN32

# include <winsock2.h>
# include <ws2tcpip.h>
# include <windows.h>

# include "ITCPSocket.h"
# include "ITCPRemoteClient.h"

class WindowsTCPRemoteClient : public ITCPRemoteClient
{
public:
	WindowsTCPRemoteClient(struct sockaddr_in &, int fd);
	virtual ~WindowsTCPRemoteClient();

	virtual int 				getWriteFd() const;
	virtual int 				getReadFd() const;
	virtual std::string const&	getIp() const;
	virtual bool 				somethingToWrite() const;

	virtual void				prepareClose();
	virtual bool 				mustBeClosed() const;

	virtual int 		operator>>(std::string &);
	virtual void		prepareMsg(std::string const &, int);
	virtual int 		send();

private:
	bool				_closing;
	std::string 		_ip;
	ITCPSocket 			*_sock;
	int 				_toSendLen;
	std::string 		_toSend;
};

#endif