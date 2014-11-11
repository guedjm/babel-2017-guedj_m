#ifndef _WIN32

#pragma once

# include "ITCPServer.h"
# include "ITCPSocket.h"
# include "ITCPRemoteClient.h"
# include "constant.h"

class LinuxTCPServer : public ITCPServer
{
public:
	LinuxTCPServer();
	virtual ~LinuxTCPServer();

	virtual int		getReadFd() const;

	virtual int		start(std::string const & = "", short = DEFAULT_SERVER_PORT, int = DEFAULT_SERVER_BACKLOG);
	virtual void	stop();

	virtual ITCPRemoteClient		*acceptClient();

private:
	ITCPSocket 		*_sock;
};
#endif
