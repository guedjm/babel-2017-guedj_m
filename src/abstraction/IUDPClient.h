#pragma once

#include <string>
#include "IObservable.h"

class IUDPClient : public IObservable
{
public:
    virtual ~IUDPClient() {};

    virtual int getReadFd() const = 0;
    virtual void closeSock() = 0;

    virtual int recvFrom(char *buff, int size, std::string &ip, unsigned short &port) = 0;
    virtual int sendTo(const char *buff, int size, std::string const &ip, unsigned short port) = 0;
};