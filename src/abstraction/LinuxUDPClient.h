#ifndef _WIN32

#pragma once

#include "IUDPClient.h"
#include "IUDPSocket.h"

class LinuxUDPClient : public IUDPClient
{
public:
    LinuxUDPClient();
    virtual ~LinuxUDPClient();

    virtual int getReadFd() const;
    virtual void closeSock();

    virtual int recvFrom(char *buff, int size, std::string &ip, unsigned short &port);
    virtual int sendTo(const char *buff, int size, std::string const &ip, unsigned short port);

private:
    IUDPSocket  *_sock;
};

#endif