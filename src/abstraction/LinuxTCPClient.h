#ifndef _WIN32

#pragma once

# include "ITCPClient.h"
# include "ITCPSocket.h"

class LinuxTCPClient : public ITCPClient
{
public:
	LinuxTCPClient();
	virtual ~LinuxTCPClient();

	virtual int 		closeSock();

	virtual int 		getWriteFd() const;
	virtual int 		getReadFd() const;
	virtual bool 		somethingToWrite() const;

	virtual int 		connectTo(std::string const &, short);

	virtual int 		operator>>(std::string &);

	virtual void 		prepareMsg(std::string const &, int);
	virtual int 		send();

private:
	int 				_toSendLen;
	std::string 		_toSend;
	ITCPSocket 			*_sock;
};

#endif