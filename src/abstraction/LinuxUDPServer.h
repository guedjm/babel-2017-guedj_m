#ifndef _WIN32

#pragma once

#include <string>
#include "IUDPSocket.h"
#include "IUDPServer.h"

class LinuxUDPServer : public IUDPServer
{
public:
    LinuxUDPServer();
    virtual ~LinuxUDPServer();

    virtual int     getReadFd() const;

    virtual int         start(std::string const &ip, unsigned short &port);
    virtual void        stop();

    virtual int         recvFrom(char *buff, int size, std::string &ip, unsigned short &port);
    virtual int         sendTo(const char *buff, int size, std::string const &ip, unsigned short port);

private:
    IUDPSocket  *_sock;
};
#endif
